// Copyleft 2024 Avis Phoenix
// SPDX-License-Identifier: GPL-3.0-only

#include "kbsavelog.h"

void KbSaveLog::createZip(const vector<string> &files, int &status, const vector<string> &podnames, string namespc)
{
    outputFile = buildZipName(podnames);
    log->info("Save Kubernetes Logs", "Trying to zip to the file: " + outputFile + ", with " + std::to_string(files.size()));
    ZipFiles zip;
    if (zip.run(outputFile, files) < 0) {
        status = -1;
        saveCMDErrorLog(namespc, "zip", zip.error());
    }
    log->info("Save Kubernetes Logs", "Zip saved");
}

void KbSaveLog::removeTempFiles(const vector<string> &files, int &status, string namespc)
{
    log->info("Save Kubernetes Logs", "Trying to remove the log files: " + std::to_string(files.size()));
    RemoveFiles rm;
    if (rm.run(files) < -1 ) {
        status = 1;
        saveCMDErrorLog(namespc, "rm", rm.error());
    }
    log->info("Save Kubernetes Logs", "Files removed");
}

int KbSaveLog::getLogFiles(string namespc, vector<string> &files, const vector<string> &podnames)
{
    int status = 0;
    for(auto &pod: podnames) {
        status += runTemp(namespc, pod);
        files.push_back(outputTemp);
    }

    return status;
}

int KbSaveLog::run(string namespc, const vector<string> &podnames){
    log->info("Save Kubernetes Logs", "Start with pods: " + std::to_string(podnames.size()));
    if (podnames.size() == 0){
        log->warning("Save Kubernetes Logs", "No pods to save");
        outputFile = "";
        return -1;
    }
    if (podnames.size() == 1){
        return run(namespc, podnames[0]);
    }
    vector<string> files;
    int status = getLogFiles(namespc, files, podnames);
    createZip(files, status, podnames, namespc);
    removeTempFiles(files, status, namespc);
    return status;
}

int KbSaveLog::run(string namespc, string podname){
    int status = runTemp(namespc, podname);
    outputFile = outputTemp;
    return status;
}

void KbSaveLog::saveErrorLog(string namespc, string podname, string errormsg) {
    std::ostringstream oss;
    oss << "Error on namespace: " << namespc << " and pod: " << podname << " Error message: " << errormsg << std::endl;
    log->error("Save Kubernetes Logs", oss.str());
}

void KbSaveLog::saveCMDErrorLog(string namespc, string cmd, string errormsg) {
    std::ostringstream oss;
    oss << "Error on namespace: " << namespc << " and command: " << cmd << " Error message: " << errormsg << std::endl;
    log->error("Save Kubernetes Logs", oss.str());
}

string KbSaveLog::getCommonName(string name1, string name2) {
    string output = "";
    bool shouldContinue = true;
    int idx = 0;

    while (shouldContinue) {
        if ((idx < name1.size() && idx < name2.size()) && name1[idx] == name2[idx] ) {
            output += name1[idx];
        } else {
            shouldContinue = false;
        }
        idx++;
    }
    return output;
}

string KbSaveLog::buildZipName(const vector<string> &podnames) {
    string outputStr = getCommonName(podnames[0], podnames[1]);
    if (outputStr.empty()) {
        outputStr = "logs";
    }
    return outputStr + ".zip";
}

int KbSaveLog::runTemp(string namespc, string podname) {
    int status = -1;
    outputTemp = podname + ".log";
    log->info("Save Kubernetes Logs", "Try to save log on " + outputTemp);
    string cmd = "kubectl -n " + namespc + " logs " + podname + " > " + outputTemp;
    ProcRunner kubectl(cmd);
    status = kubectl.run();
    if (status < 0) {
        saveErrorLog(namespc, podname, kubectl.error());
    }
    log->info("Save Kubernetes Logs", "Saved log: " + outputTemp );
    return status;
}
