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
 * @file zipfiles.h
 * @brief Execute the zip command to zip files.
 * @author Avis Phoenix
 */

#ifndef ZIPFILES_H
#define ZIPFILES_H

#include "../procrunner.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

class ZipFiles
{
public:
    ZipFiles() : zip("zip") {}
    int run(string zipname, const vector<string> &files) {
        vector<string> args({zipname});
        args.insert(args.end(), files.begin(), files.end());
        return zip.run(&args);
    }
    int run(string zipfile, string pattern) {
        vector<string> args = { zipfile, pattern };
        return zip.run(&args);
    }
    string error() { return zip.error(); }
private:
    ProcRunner zip;
};

#endif // ZIPFILES_H
