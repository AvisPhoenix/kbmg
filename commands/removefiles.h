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
 * @file removefiles.h
 * @brief delete files (using rm command, so only compatible with posix)
 * @author Avis Phoenix
 */
#ifndef REMOVEFILES_H
#define REMOVEFILES_H

#include "../procrunner.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

class RemoveFiles
{
public:
    RemoveFiles() : rm("rm") {}
    int run(const vector<string> &files) {
        return rm.run(&files);
    }
    int run(string file){
        vector<string> args({ file });
        return run(args);
    }
    string error() {
        return rm.error();
    }
private:
    ProcRunner rm;
};

#endif // REMOVEFILES_H
