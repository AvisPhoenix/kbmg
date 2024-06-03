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
 * @file kbgoenvironment.h
 * @brief Execute the command to change the environment, in this case az
 * @author Avis Phoenix
 */

#ifndef KBGOENVIRONMENT_H
#define KBGOENVIRONMENT_H

#include "../procrunner.h"
#include "../logger.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

class KbGoEnvironment
{
public:
    KbGoEnvironment(Logger *log) : az("az") {
        this->log = log;
    }
    inline void saveErrorLog(string namespc, string errormsg) {
        std::ostringstream oss;
        oss << "Error trying to change to namespace: " << namespc <<". Error message: " << errormsg << std::endl;
        log->error("Save Kubernetes Logs", oss.str());
    }

    int run(string env) {
        string resourceGroup = "RG-" + env;
        string aksName = "aks-" + env;
        vector<string> arguments = { "aks", "get-credentials", "--resource-group", resourceGroup, "--name", aksName };
        int status = az.run(&arguments);
        if (status < 0) {
            saveErrorLog(env, az.error() );
        }
        return status;
    }
private:
    ProcRunner az;
    Logger *log;
};

#endif // KBGOENVIRONMENT_H
