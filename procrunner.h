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
 * @file procrunner.h
 * @brief Procedure Runner, let you run a external command and get his output or error messages
 * @author Avis Phoenix
 */

#ifndef PROCRUNNER_H
#define PROCRUNNER_H

#include <string>
#include <vector>
#include <optional>
#include <pstreams/pstream.h>
#include <fstream>

using std::string;

class ProcRunner
{
public:
    ProcRunner(string cmd);
    string output() { return outputStr; }
    string error() { return errorStr; }
    int status() { return status_code; }
    int run(const std::vector<string> *argv = nullptr);

private:
    string command;
    string outputStr;
    string errorStr;

    int status_code;
    redi::ipstream buildProcedure(const std::vector<string> *argv = nullptr);
    void readOutput(redi::ipstream &process, bool finished[], string &line);
    void readErrors(redi::ipstream &process, bool finished[], string &line);
    void saveCommandOutput(redi::ipstream &process);
    void resetOutput();
    void save2RAM(bool finished[], redi::ipstream &process, string &line);
};

#endif // PROCRUNNER_H
