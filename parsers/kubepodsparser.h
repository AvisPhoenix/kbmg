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
 * @file kubepodsparser.h
 * @brief Read all the information from kubectl get pods
 * @author Avis Phoenix
 */

#ifndef KUBEPODSPARSER_H
#define KUBEPODSPARSER_H


#include <string>
#include <sstream>
#include <ostream>
#include <vector>

using std::string;
using std::vector;

class PodsInfo
{
public:
    string name;
    string ready;
    string status;
    string restarts;
    string age;

    PodsInfo(){}
};

inline std::ostream& operator<<(std::ostream& out, const PodsInfo& obj){
    out << "{ Name: " << obj.name << ", Ready: " << obj.ready
        << ", Status: " << obj.status << ", Restart: " << obj.restarts
        << ", Age: " << obj.age <<" }";
    return out;
}

class KubePodsParser
{
public:
    KubePodsParser(string &output);
    vector<PodsInfo> &pods(){ return podList; }
private:
    vector<PodsInfo> podList;
    vector<string> errorList;
    vector<string> split(const string &s, char delim);
    vector<string> getPodInfo(const string &line);
};



#endif // KUBEPODSPARSER_H
