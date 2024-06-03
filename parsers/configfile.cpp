// Copyleft 2024 Avis Phoenix
// SPDX-License-Identifier: GPL-3.0-only

#include "configfile.h"

ConfigFile::ConfigFile():
    log("Log"),env("Env"), namespc("Namespace"), podshort("PodAlias"),
    exec("Exec"), execpod("Pod"), execname("Namespace"), execdef("Default"){
    try {
        config = YAML::LoadFile("config.yaml");
        isLoaded = true;
    } catch (const std::exception& e){
        isLoaded = false;
    }
}

string ConfigFile::getLogFilePath(){
    string path = "kbmg.log";
    if (isLoaded && hasValue(log)) {
        string value = config[log].as<string>();
        if (!value.empty()) {
            path = value;
        }
    }

    return path;
}

map<string, string> ConfigFile::getEnv(){
    map<string, string> mapOut;

    if (isLoaded && hasValue(env)) {
        mapOut = config[env].as<map<string, string>>();
    }

    return mapOut;
}

map<string, string> ConfigFile::getNamespaces(){
    map<string, string> mapOut;

    if (isLoaded && hasValue(namespc)) {
        mapOut = config[namespc].as<map<string, string>>();
    }

    return mapOut;
}

map<string, string> ConfigFile::getAliasPods(){
    map<string, string> mapOut;

    if (isLoaded && hasValue(podshort)) {
        mapOut = config[podshort].as<map<string, string>>();
    }

    return mapOut;
}

string ConfigFile::getExecDefault() {
    string defaultExec = "bash";
    if (isLoaded && hasValue(exec) && hasValue(execdef, config[exec])) {
        defaultExec = config[exec][execdef].as<string>();
        toLower(defaultExec);
        if (defaultExec.compare("bash") != 0 && defaultExec.compare("sh") != 0){
            defaultExec = "bash";
        }
    }

    return defaultExec;
}

void ConfigFile::validateExecValues(map<string, string> &mapOut)
{
    std::vector<string> invalidKeys;
    string command;
    for (auto const& [key, val] : mapOut) {
        command = val;
        toLower(command);
        mapOut[key] = command;
        if (command.compare("bash") != 0 && command.compare("sh") != 0){
            invalidKeys.push_back(key);
        }
    }
    for(auto &key : invalidKeys) {
        mapOut.erase(key);
    }
}

map<string, string> ConfigFile::getExecByNamespace() {
    map<string, string> mapOut;

    if (isLoaded && hasValue(exec) && hasValue(execname, config[exec])) {
        mapOut = config[exec][execname].as<map<string, string>>();
    }
    validateExecValues(mapOut);

    return mapOut;
}

map<string, string> ConfigFile::getExecByPodName() {
    map<string, string> mapOut;

    if (isLoaded && hasValue(exec) && hasValue(execpod, config[exec])) {
        mapOut = config[exec][execpod].as<map<string, string>>();
    }
    validateExecValues(mapOut);

    return mapOut;
}

string ConfigFile::help() {
    std::ostringstream oss;
    oss << "Config file use a YAML Format, which should have the next properties:\n"
        << "\t" << log << ": <file path>\n the full path where the log file should be saved.\n"
        << "\t" << env << ":\n\t\t<env name> : <value>\n env name: is the name you should use in the application and value is the name of the environment to kubernetes.\n"
        << "\t" << namespc << ":\n\t\t<namespace alias> : <namespace name>\n namespace alias is the name you should use in the application and the namespace is the name in kubernetes.\n"
        << "\t" << podshort << ":\n\t\t<pod alias> : <pod name>\n pod alias is the name you should use in the application and the pod name is part of the pod name in kubernetes.\n"
        << "\t" << exec << ":\n\t\t" << execdef << ": <bash or sh> \t Choose the defult command interface, if does not exist will be bash.\n"
        << "\t\t" << execname << ":\n\t\t\t<namespace> : <bash or sh>\n the namespace is the name in kubernetes, if does not define will be the default value.\n"
        << "\t\t" << execpod << ":\n\t\t\t<pod name> : <bash or sh>\n the pod name is part of the name on kubernetes, if does not define will be the default value.\n"
        << "For " << exec << " section, the pod name is prior than the namespace and at the end is the default rule.\n"
        << "EXAMPLE:\n"
        << log << ": /tmp/kbmg.log\n"
        << env << ":\n"
        << "\tdev: NAME_DEV\n"
        << "\tqa: NAME_QA\n"
        << namespc << ":\n"
        << "\tA: namespacea\n"
        << podshort << ":\n"
        << "\tmypod: microservicepod\n"
        << exec << ":\n"
        << "\t" << execname << ":\n"
        << "\t\tnamespacea: sh\n"
        << "\t" << execpod << ":\n"
        << "\t\tmicroservicepod: sh\n";
    return oss.str();
}

bool ConfigFile::hasValue(string key, YAML::Node node) {
    bool exist = false;
    for(const auto& child : node) {
        string k = child.first.as<string>();
        if (key.compare(k) == 0) {
            exist = true;
        }
    }
    return exist;
}

bool ConfigFile::hasValue(string key) {
    return hasValue(key, config);
}

void ConfigFile::toLower(string &str) {
    std::locale loc;
    for(int i =0; i < str.size(); i++) {
        str[i] = std::tolower(str[i], loc);
    }
}
