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
 * @file kbgoshell.h
 * @brief Build kubectl exec with sh command to be executed by user
 * @author Avis Phoenix
 */
#ifndef KBGOSHELL_H
#define KBGOSHELL_H

#include <string>
#include <vector>

using std::string;
using std::vector;


class KbGoShell
{
public:
    KbGoShell( ) {}
    string build(string namespc, const vector<string> &podnames) {
        std::ostringstream oss;
        for(auto &pod: podnames) {
            oss << "kubectl -n " << namespc <<" exec -it " << pod << "  p -- sh" << std::endl;
        }
        return oss.str();
    }
    string build(string namespc, const string &pod) {
        std::ostringstream oss;
        oss << "kubectl -n " << namespc <<" exec -it " << pod << "  p -- sh" << std::endl;
        return oss.str();
    }
};

#endif // KBGOSHELL_H
