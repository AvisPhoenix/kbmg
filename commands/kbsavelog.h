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
 * @file kbsavelog.h
 * @brief Execute the command to download the logs of a pod, if there is more than one pod generate the zip and remove the generated files
 * @author Avis Phoenix
 */

#ifndef KBSAVELOG_H
#define KBSAVELOG_H

#include "../procrunner.h"
#include "../logger.h"
#include "zipfiles.h"
#include "removefiles.h"
#include <string>
#include <sstream>
#include <vector>

using std::string;
using std::vector;

class KbSaveLog
{
public:
    KbSaveLog(Logger *log){
        this->kubectl = new ProcRunner("kubectl");
        this->log = log;
    }
    ~KbSaveLog(){
        delete kubectl;
    }
    int run(string namespc, const vector<string> &podnames);
    int run(string namespc, string podname);
    string output() { return outputFile; }
private:
    ProcRunner *kubectl;
    Logger *log;
    string outputFile;
    string outputTemp;
    int runTemp(string namespc, string podname);
    void saveErrorLog(string namespc, string podname, string errormsg);
    void saveCMDErrorLog(string namespc, string cmd, string errormsg);
    string getCommonName(string name1, string name2);
    string buildZipName(const vector<string> &podnames);
    void createZip(const vector<string> &files, int &status,const vector<string> &podnames, string namespc);
    void removeTempFiles(const vector<string> &files, int &status, string namespc);
    int getLogFiles(string namespc, vector<string> &files, const vector<string> &podnames);
};

#endif // KBSAVELOG_H
