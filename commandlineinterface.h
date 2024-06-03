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
 * @file commandlineinterface.h
 * @brief Command Line Interface for the user, manage all the commands by arguments
 * @author Avis Phoenix
 */

#ifndef COMMANDLINEINTERFACE_H
#define COMMANDLINEINTERFACE_H

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "kubernetesmanager.h"
#include "parsers/configfile.h"
#include "logger.h"

using std::string;
using std::vector;
using std::map;
using std::cout;
using std::endl;

enum class K8Action { GetPods, GetLogs, DownloadLogs, Exec, Unknown, Help, ConfigHelp };
enum class CmdExec { Bash, Shell };

class CommandLineInterface
{
public:
    CommandLineInterface(KubernetesManager *kbMng, ConfigFile *config, Logger *log);
    void execArguments(const vector<string> &args);
    bool UnknownAction() { return action == K8Action::Unknown; }
    void printHelp();

private:
    const vector<string> actions;
    const string namespcCmd, podCmd, envCmd, helpCmd, configHelpCmd;
    KubernetesManager *kbMng;
    Logger *log;
    ConfigFile *config;

    K8Action action;
    string namespc, pod, env;

    void parseArguments(const vector<string> &args);
    void updateAlias();
    CmdExec command2Exec();
    void runCommand();
    string help();
    void printConfigHelp();

    void updatePodAlias();
    void updateNamespaceAlias();
    CmdExec checkPodRule();
    CmdExec checkNamespaceRule();
    vector<string> filterPods();
    bool validatePods(const vector<PodsInfo> &currentPods);
    void move2Env();
    void updateEnvironmentName();
    void noEnvError();
    void showPods();
    void downloadAction(const vector<string> &pods);
    void showLogCmd(const vector<string> &pods);
    void buildExecCmds(const vector<string> &pods);
    void runPodsComd(void (CommandLineInterface::*function)(const vector<string>&));
    CmdExec getDefaultExec();
    vector<int> getExplicitOptions(const vector<string> &args);
    void resetValues();
    string getArgValue(const vector<string> &args, int &idx, vector<int> &check);
    void getUnexplicitOptions(const vector<int> &usedIdx, const vector<string> &args);
    vector<int> buildUnusedIndexes(const vector<int> &usedIdx, const vector<string> &args);
    bool validatePod();
    void K8Action2String();
    string K8Actions2String();
};

class NoRule : public std::exception {
public:
    const char *what() const noexcept { return "Does not found rule"; }
};

class NoEnv : public std::exception {
public:
    const char *what() const noexcept { return "Does not found environment"; }
};


#endif // COMMANDLINEINTERFACE_H
