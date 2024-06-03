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
 * @file kbgobash.h
 * @brief Build kubectl exec with bash command to be executed by user
 * @author Avis Phoenix
 */
#ifndef KBGOBASH_H
#define KBGOBASH_H

#include <string>
#include <sstream>
#include <vector>

using std::string;
using std::vector;

class KbGoBash
{
public:
    KbGoBash(){ }
    string build(string namespc, const vector<string> &podnames) {
        std::ostringstream oss;
        for(auto &pod: podnames) {
            oss << "kubectl -n " << namespc <<" exec -it " << pod << "  p -- bash" << std::endl;
        }
        return oss.str();
    }
    string build(string namespc, const string &pod) {
        std::ostringstream oss;
        oss << "kubectl -n " << namespc <<" exec -it " << pod << "  p -- bash" << std::endl;
        return oss.str();
    }
};

#endif // KBGOBASH_H
