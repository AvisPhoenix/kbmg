// Copyleft 2024 Avis Phoenix
// SPDX-License-Identifier: GPL-3.0-only

#include "kubernetesmanager.h"

KubernetesManager::KubernetesManager(Logger *logger) : kbGoEnv(logger), kbSaveLogs(logger) {
    kubectl = new ProcRunner("kubectl");
    kbGetPods = new KbGetPods(kubectl, logger);
    log = logger;
}

KubernetesManager::~KubernetesManager() {
    delete kubectl;
    delete kbGetPods;
}

string KubernetesManager::downloadLogs(string namespc, const vector<string> &pods) {
    log->info("KubernetesManager", "Try to Download Logs");
    if (kbSaveLogs.run(namespc, pods) < 0) {
        string msg = "Occours an error while trying to download logs. See the log for more details.";
        log->error("KbMg Download Logs", msg);
        throw std::runtime_error(msg);
    }
    log->info("KubernetesManager", "executed download logs on: " + kbSaveLogs.output());
    return kbSaveLogs.output();
}

string KubernetesManager::cmdLogs(string namespc, const vector<string> &pods) {
    return kbGetLog.build(namespc, pods);
}

string KubernetesManager::cmdExecBash(string namespc, const vector<string> &pods) {
    return kbGoBash.build(namespc, pods);
}

string KubernetesManager::cmdExecSh(string namespc, const vector<string> &pods) {
    return kbGoShell.build(namespc, pods);
}

string KubernetesManager::cmdExecBash(string namespc, const string &pod) {
    return kbGoBash.build(namespc, pod);
}

string KubernetesManager::cmdExecSh(string namespc, const string &pod) {
    return kbGoShell.build(namespc, pod);
}

string KubernetesManager::displayPods(string namespc) {
    log->info("KubernetesManager", "Try to display pods");
    if (kbGetPods->run(namespc) < 0) {
        string msg = "Occours an error while trying to display the pods in " + namespc + " namespace. See the log for more details.";
        log->error("KbMg Display Pods", msg);
        throw std::runtime_error(msg);
    }
    log->info("KubernetesManager", "executed display pods");
    return kbGetPods->output();
}

vector<PodsInfo> KubernetesManager::getPods(string namespc) {
    log->info("KubernetesManager", "Try to get pods");
    if (kbGetPods->run(namespc) < 0) {
        string msg = "Occours an error while trying to get the pods in " + namespc + " namespace. See the log for more details.";
        log->error("KbMg Get Pods", msg);
        throw std::runtime_error(msg);
    }
    log->info("KubernetesManager", "executed get pods");
    return kbGetPods->pods();
}

int KubernetesManager::goEnv(string env) {
    log->info("KubernetesManager", "Try to change Environment");
    int status = kbGoEnv.run(env);
    if (status < 0) {
        string msg = "Occours an error while trying to change the env into " + env + ". See the log for more details.";
        log->error("KbMg Go Env", msg);
        throw std::runtime_error(msg);
    }
    log->info("KubernetesManager", "executed go Environment");
    return status;
}
