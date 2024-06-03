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
 * @file kbgetlogs.h
 * @brief Build kubectl logs command to be executed by user
 * @author Avis Phoenix
 */
#ifndef KBGETLOG_H
#define KBGETLOG_H

#include <string>
#include <sstream>
#include <vector>

using std::string;
using std::vector;

class KbGetLog
{
public:
    KbGetLog() {}
    string build(string namespc, const vector<string> &podnames) {
        std::ostringstream oss;
        int idx;
        for(idx = 0; idx < podnames.size() -1 ; idx++) {
            oss << "kubectl -n " << namespc <<" logs " << podnames[idx] << " && ";
        }
        if (idx < podnames.size()){
            oss << "kubectl -n " << namespc <<" logs " << podnames[idx] << std::endl;
        }
        return oss.str();
    }
};

#endif // KBGETLOG_H
