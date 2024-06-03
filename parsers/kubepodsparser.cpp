// Copyleft 2024 Avis Phoenix
// SPDX-License-Identifier: GPL-3.0-only

#include "kubepodsparser.h"

KubePodsParser::KubePodsParser(string &output) {
    vector<string> lines = split(output, '\n');
    lines.erase(std::begin(lines));
    for(auto &line: lines){
        vector<string> values = getPodInfo(line);
        if (values.size() != 5){
            /// To-Do: Log error
        }else {
            PodsInfo pod;
            pod.name = values[0];
            pod.ready = values[1];
            pod.status = values[2];
            pod.restarts = values[3];
            pod.age = values[4];

            podList.push_back(pod);
        }
    }
}

vector<string> KubePodsParser::split(const string &s, char delim) {
    vector<string> result;
    std::stringstream ss (s);
    string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}

vector<string> KubePodsParser::getPodInfo(const string &line) {
    vector<string> values;
    string currentValue;
    for(int i = 0; i < line.size(); i++){
        if (line[i] == ' '){
            if(line[i+1] == ' ' && !currentValue.empty()){
                values.push_back(currentValue);
                currentValue.clear();
            } else if (line[i+1] != ' ' && !currentValue.empty()){
                currentValue += " ";
            }
        } else{
            currentValue += line[i];
        }
    }
    if (values.size() < 5){
        values.push_back(currentValue);
    }
    return values;
}

