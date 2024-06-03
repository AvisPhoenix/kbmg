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
 * @file main.cpp
 * @brief Main loop
 * @author Avis Phoenix
 */

#include <iostream>
#include <vector>
#include <string>
#include "parsers/configfile.h"
#include "kubernetesmanager.h"
#include "logger.h"
#include "commandlineinterface.h"

using namespace std;


vector<string> getArguments(char* argv[], int argc)
{
    vector<string> args;
    for(int i=1; i < argc; i++) {
        string argument(argv[i]);
        if (!argument.empty()) {
            args.push_back(argument);
        }
    }

    return args;
}

int main(int argc, char* argv[]) {
    vector<string> args = getArguments(argv, argc);
    int status = 0;
    try {
        ConfigFile config;
        Logger myLog(config.getLogFilePath());
        KubernetesManager kbMng(&myLog);
        CommandLineInterface cli(&kbMng, &config, &myLog);
        cli.execArguments(args);
        if (cli.UnknownAction()) {
            cli.printHelp();
        }
    } catch (std::exception e){
        status = -1;
        cout << "Unexpected error: " << e.what() << endl;
    }

    return status;
}

