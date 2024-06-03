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
 * @file configfile.h
 * @brief Read the configuration file
 * @author Avis Phoenix
 */
#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <yaml-cpp/yaml.h>
#include <map>
#include <vector>
#include <sstream>
#include <locale>

using std::string;
using std::map;

class ConfigFile
{
public:
    ConfigFile();
    string getLogFilePath();
    map<string, string> getEnv();
    map<string, string> getNamespaces();
    map<string, string> getAliasPods();
    string getExecDefault();
    map<string, string> getExecByNamespace();
    map<string, string> getExecByPodName();
    string help();
private:
    const string log, env, namespc, podshort, exec, execpod, execname, execdef;
    bool isLoaded;
    YAML::Node config;
    bool hasValue(string key, YAML::Node node );
    bool hasValue(string key);
    void toLower(string &str);
void validateExecValues(map<string, string> &mapOut);
};

#endif // CONFIGFILE_H
