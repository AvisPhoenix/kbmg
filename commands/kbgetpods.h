/* 
 * This file is part of the Kubernetes Manager (https://github.com/AvisPhoenix/kbmg).
 * Copyright (c) 2024 Avis Phoenix.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
/**
 * @file kbgetpods.h
 * @brief Run kubectl get pods command
 * @author Avis Phoenix
 */
#ifndef KBGETPODS_H
#define KBGETPODS_H

#include "../procrunner.h"
#include "../parsers/kubepodsparser.h"
#include "../logger.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

class KbGetPods
{
public:
    KbGetPods(ProcRunner *kubectl, Logger *log) {
        this->kubectl = kubectl;
        parserOutput = nullptr;
        this->log = log;
    }
    ~KbGetPods(){
        if (parserOutput != nullptr)
            delete parserOutput;
    }
    int run(string namespc);
    vector<PodsInfo> pods();
    string output() { return cmdOutput; }
private:
    ProcRunner *kubectl;
    Logger *log;
    KubePodsParser *parserOutput;
    string cmdOutput;
    void saveErrorLog(string namespc);
};

#endif // KBGETPODS_H
