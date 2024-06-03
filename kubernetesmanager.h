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
 * @file kubernetesmanager.h
 * @brief Flow manager of every action
 * @author Avis Phoenix
 */
#ifndef KUBERNETESMANAGER_H
#define KUBERNETESMANAGER_H

#include <string>
#include <vector>
#include <exception>
#include "parsers/configfile.h"
#include "parsers/kubepodsparser.h"
#include "commands/kbgetpods.h"
#include "commands/kbgoenvironment.h"
#include "commands/kbsavelog.h"
#include "commands/builder/kbgetlog.h"
#include "commands/builder/kbgobash.h"
#include "commands/builder/kbgoshell.h"
#include "logger.h"
#include "procrunner.h"

using std::string;
using std::vector;

class KubernetesManager
{
public:
    KubernetesManager(Logger *logger);
    ~KubernetesManager();
    string downloadLogs(string namespc, const vector<string> &pod);
    string cmdLogs(string namespc, const vector<string> &pod);
    string cmdExecBash(string namespc, const vector<string> &pods);
    string cmdExecSh(string namespc, const vector<string> &pods);
    string cmdExecBash(string namespc, const string &pod);
    string cmdExecSh(string namespc, const string &pod);
    string displayPods(string namespc);
    vector<PodsInfo> getPods(string namespc);
    int goEnv(string env);
private:
    Logger *log;
    ProcRunner *kubectl;
    KbGetLog kbGetLog;
    KbGetPods *kbGetPods;
    KbGoBash kbGoBash;
    KbGoShell kbGoShell;
    KbGoEnvironment kbGoEnv;
    KbSaveLog kbSaveLogs;
};

#endif // KUBERNETESMANAGER_H
