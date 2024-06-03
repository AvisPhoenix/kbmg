// Copyleft 2024 Avis Phoenix
// SPDX-License-Identifier: GPL-3.0-only

#include "kbgetpods.h"
#include <iostream>

void KbGetPods::saveErrorLog(string namespc)
{
    std::ostringstream oss;
    oss << "Error trying to get the pods on namespace: " << namespc << " Error message: " << kubectl->error() << std::endl;
    log->error("Save Kubernetes Logs", oss.str());
}

int KbGetPods::run(string namespc) {
    vector<string> args({"get", "pods", "-n", namespc});
    int status = kubectl->run(&args);
    cmdOutput = kubectl->output();
    if (parserOutput != nullptr) {
        delete parserOutput;
        parserOutput = nullptr;
    }
    parserOutput = new KubePodsParser(cmdOutput);
    if (status < 0) {
        saveErrorLog(namespc);
    }
    return status;
}

vector<PodsInfo> KbGetPods::pods() {
    if (parserOutput == nullptr){
        return vector<PodsInfo>();
    } else {
        return parserOutput->pods();
    }
}
