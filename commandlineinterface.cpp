// Copyleft 2024 Avis Phoenix
// SPDX-License-Identifier: GPL-3.0-only

#include "commandlineinterface.h"

CommandLineInterface::CommandLineInterface(KubernetesManager *kbMng, ConfigFile *config, Logger *log):
    actions({"pods", "log", "dlog", "exec"}),
    namespcCmd("-n"), podCmd("-p"), envCmd("-e"), helpCmd("-help"), configHelpCmd("-howConf"){
    this->kbMng = kbMng;
    this->log = log;
    this->config = config;
    action = K8Action::Unknown;
}

bool CommandLineInterface::validatePod()
{
    bool valid = true;
    if (action != K8Action::GetPods){
        if (pod.empty()){
            cout << "You need to specify the pod name. See " << helpCmd << " for more details." << endl;
            valid = false;
        }
    }
    return valid;
}

string CommandLineInterface::K8Actions2String()
{
    string actionStr = "";
    switch (action) {
    case K8Action::ConfigHelp:
        actionStr = "Ask config help";
        break;
    case K8Action::DownloadLogs:
        actionStr = "Download Logs";
        break;
    case K8Action::Exec:
        actionStr = "Execute";
        break;
    case K8Action::GetLogs:
        actionStr = "Ask for Logs commands";
        break;
    case K8Action::GetPods:
        actionStr = "Ask for pods";
        break;
    case K8Action::Help:
        actionStr = "Ask for help";
        break;
    case K8Action::Unknown:
        actionStr = "Unknowm Action";
        break;
    }

    return actionStr;
}

void CommandLineInterface::execArguments(const vector<string> &args) {
    log->info("CLI", "Start process");
    parseArguments(args);
    if (action != K8Action::Unknown && action != K8Action::Help && action != K8Action::ConfigHelp && validatePod()) {
        updateAlias();
        log->info("CLI", "action: " + K8Actions2String() + ", env: " + env + ", namespace: " + namespc + ", pod: " + pod);
        runCommand();
    } else if (action == K8Action::Help){
        printHelp();
    } else if (action == K8Action::ConfigHelp) {
        printConfigHelp();
    }
}

string CommandLineInterface::getArgValue(const vector<string> &args, int &idx, vector<int> &check)
{
    check.push_back(idx);
    idx++;
    check.push_back(idx);
    return args[idx];
}

vector<int> CommandLineInterface::getExplicitOptions(const vector<string> &args) {
    int idx = 0;
    int index = 0;
    vector<int> check;
    while (idx < args.size()){
        auto it = std::find(actions.begin(), actions.end(), args[idx]);
        if (it != actions.end()){
            index = it - actions.begin();
            switch (index) {
            case 0:
                action = K8Action::GetPods;
                break;
            case 1:
                action = K8Action::GetLogs;
                break;
            case 2:
                action = K8Action::DownloadLogs;
                break;
            case 3:
                action = K8Action::Exec;
                break;
            }
            check.push_back(idx);
        }
        if (helpCmd.compare(args[idx]) == 0) {
            action = K8Action::Help;
            check.push_back(idx);
        }
        if (configHelpCmd.compare(args[idx]) == 0) {
            action = K8Action::ConfigHelp;
            check.push_back(idx);
        }
        if (namespcCmd.compare(args[idx]) == 0) {
            namespc = getArgValue(args, idx, check);
        }
        if (podCmd.compare(args[idx]) == 0) {
            pod = getArgValue(args, idx, check);
        }
        if (envCmd.compare(args[idx]) == 0) {
            env = getArgValue(args, idx, check);
        }
        idx++;
    }

    return check;
}

void CommandLineInterface::resetValues() {
    namespc = "";
    pod = "";
    env = "";
    action = K8Action::Unknown;
}

vector<int> CommandLineInterface::buildUnusedIndexes(const vector<int> &usedIdx, const vector<string> &args)
{
    vector<int> unusedIdx;
    for (int idx = 0; idx < args.size(); idx++) {
        if (std::find(usedIdx.begin(), usedIdx.end(), idx) == usedIdx.end()) {
            unusedIdx.push_back(idx);
        }
    }

    return unusedIdx;
}

void CommandLineInterface::getUnexplicitOptions(const vector<int> &usedIdx, const vector<string> &args)
{
    vector<int> unusedIdx = buildUnusedIndexes(usedIdx, args);
    for (auto &idx : unusedIdx) {
        if (env.empty()) {
            env = args[idx];
        } else if(namespc.empty()) {
            namespc = args[idx];
        } else if (pod.empty()) {
            pod = args[idx];
        } else {
            cout << "Argument \"" << args[idx] << "\" will be ignored." << endl;
            log->warning("CLI Reading Arguments", "Ignored argument: " + args[idx]);
        }
    }
}

void CommandLineInterface::parseArguments(const vector<string> &args) {
    resetValues();
    vector<int> usedIdx = getExplicitOptions(args);
    getUnexplicitOptions(usedIdx, args);
}

void CommandLineInterface::updatePodAlias()
{
    map<string, string> podAlias = config->getAliasPods();
    if (podAlias.find(pod) != podAlias.end()) {
        pod = podAlias[pod];
    }
}

void CommandLineInterface::updateNamespaceAlias()
{
    map<string,string> nspcAlias = config->getNamespaces();
    if (nspcAlias.find(namespc) != nspcAlias.end()) {
        namespc = nspcAlias[namespc];
    }
}

void CommandLineInterface::noEnvError()
{
    string msg = "No found environment named \"" + env + "\" on config file.";
    cout << msg << endl;
    log->error("CLI Found Env", msg);
    throw NoEnv();
}

void CommandLineInterface::updateEnvironmentName()
{
    map<string,string> envValues = config->getEnv();
    if (envValues.find(env) != envValues.end()) {
        env = envValues[env];
    } else {
        noEnvError();
    }
}

void CommandLineInterface::updateAlias() {
    updateEnvironmentName();
    updatePodAlias();
    updateNamespaceAlias();
}

bool CommandLineInterface::validatePods(const vector<PodsInfo> &currentPods) {
    bool existPods = currentPods.size() > 0;
    if (!existPods) {
        std::ostringstream oss;
        oss << "No pods on this current namespace " << namespc << " and environment " << env << ". Please check your config file or kubernetes.";
        string msg = oss.str();
        cout << msg << endl;
        log->error("CLI Get Pods", msg);
    }
    return existPods;
}

void CommandLineInterface::move2Env() {
    kbMng->goEnv(env);
}

vector<string> CommandLineInterface::filterPods() {
    vector<string> output;
    vector<PodsInfo> currentPods = kbMng->getPods(namespc);
    if (validatePods(currentPods)){
        for(auto &podInfo: currentPods) {
            if (podInfo.name.find(pod) != string::npos && podInfo.status.compare("Running") == 0) {
                output.push_back(podInfo.name);
            }
        }
    }
    return output;
}

CmdExec CommandLineInterface::checkPodRule() {
    map<string, string> podRule = config->getExecByPodName();
    if (podRule.find(pod) != podRule.end()) {
        string cmd = podRule[pod];
        if (cmd.compare("bash") == 0){
            return CmdExec::Bash;
        } else {
            return CmdExec::Shell;
        }
    }
    throw NoRule();
}

CmdExec CommandLineInterface::checkNamespaceRule() {
    map<string, string> namespcRule = config->getExecByNamespace();
    if (namespcRule.find(namespc) != namespcRule.end()) {
        string cmd = namespcRule[namespc];
        if (cmd.compare("bash") == 0){
            return CmdExec::Bash;
        } else {
            return CmdExec::Shell;
        }
    }
    throw NoRule();
}

CmdExec CommandLineInterface::getDefaultExec()
{
    if (config->getExecDefault().compare("bash") == 0) {
        return CmdExec::Bash;
    } else {
        return CmdExec::Shell;
    }
}

CmdExec CommandLineInterface::command2Exec() {
    try {
        return checkPodRule();
    } catch(NoRule) {
        try {
            return checkNamespaceRule();
        } catch(NoRule) {
            return getDefaultExec();
        }
    }
}

void CommandLineInterface::showPods() {
    move2Env();
    cout << kbMng->displayPods(namespc) << endl;
}

void CommandLineInterface::runCommand() {
    switch (action) {
    case K8Action::ConfigHelp:
        log->info("CLI", "Show Config Help");
        printConfigHelp();
        break;
    case K8Action::DownloadLogs:
        log->info("CLI", "Download Action");
        runPodsComd(&CommandLineInterface::downloadAction);
        break;
    case K8Action::Exec:
        log->info("CLI", "Execute");
        runPodsComd(&CommandLineInterface::buildExecCmds);
        break;
    case K8Action::GetLogs:
        log->info("CLI", "Build Log commands");
        runPodsComd(&CommandLineInterface::showLogCmd);
        break;
    case K8Action::GetPods:
        log->info("CLI", "Show Pods");
        showPods();
        break;
    case K8Action::Help:
        log->info("CLI", "Show Help");
        printHelp();
        break;
    }
}

string CommandLineInterface::help() {
    std::ostringstream oss;
    oss << "Kubernetes Manager (v0.5.5) -- Help" << endl
        << "Basic Usage: " << endl
        << "kbmg <environment> <namespace> <action> <pod?>" << endl
        << endl << "Example:" << endl
        << "kbmg dev spc " << actions[2] << " db-operation"  << endl
        << "*Notes*: <pod> is not required for " << actions[0] << " action." << endl
        << "         <env> should be on the config file." << endl
        << endl << "Actions: " << endl
        << "\t" << actions[0] << ": Get all the pods for that namespace and environment." << endl
        << "\t" << actions[1] << ": Get the logs commands for all the pods which has name as part of his name, for that namespace and environment." << endl
        << "\t" << actions[2] << ": Download all the logs of all the pods which has name as part of his name, for that namespace and environment. (if there is more than one, you will get a zip file, and shows the name." << endl
        << "\t" << actions[3] << ": Get the exec command for all the pods which has name as part of his name, for that namespace and environment. (this will follows the rules of config file, by default bash)" << endl
        << endl << "Options (If you want to use other order of each option or be explicit): " << endl
        <<  "\t" << envCmd << " <env> \t Define the environment you want to use." << endl
        << "\t" <<  namespcCmd << " <namespace> \t Define the namespace you want to use." << endl
        << "\t" <<  podCmd << " <pod> \t Define the pod alias or part of the pod name you want to use." << endl
        << "Example with options:" << endl
        << "\t" <<  "kbmg " << actions[3] << " " << namespcCmd << " space " << podCmd << " db " << envCmd << " dev" << endl
        << endl << "Special commands: " << endl
        << "\t" << helpCmd << ": Show this screen." << endl
        << "\t" << configHelpCmd << ": Show help to create your config file. ";
    return oss.str();
}

void CommandLineInterface::printConfigHelp() {
    cout << config->help() << endl;
}

void CommandLineInterface::printHelp() {
    cout << help() << endl;
}

void CommandLineInterface::downloadAction(const vector<string> &pods) {
    cout << kbMng->downloadLogs(namespc, pods) << endl;
}

void CommandLineInterface::showLogCmd(const vector<string> &pods) {
    cout << kbMng->cmdLogs(namespc, pods) << endl;
}

void CommandLineInterface::buildExecCmds(const vector<string> &pods) {
    switch (command2Exec()) {
    case CmdExec::Bash:
        cout << kbMng->cmdExecBash(namespc, pods) << endl;
        break;
    case CmdExec::Shell:
        cout << kbMng->cmdExecSh(namespc, pods) << endl;
        break;
    }
}

void CommandLineInterface::runPodsComd(void (CommandLineInterface::*function)(const vector<string>&)) {
    move2Env();
    vector<string> pods = filterPods();
    if (pods.size() == 0){
        cout << "Cannot find pods with : " << pod << " as part of his name. " << endl;
        log->warning("CLI", "No pods");
    } else {
        (this->*function)(pods);
    }
}
