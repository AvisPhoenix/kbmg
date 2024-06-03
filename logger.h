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
 * @file logger.h
 * @brief Write the log file
 * @author Avis Phoenix
 */
#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include <chrono>
#include <ctime>

class Logger
{
public:
    Logger(std::string filename);
    void info(std::string parent, std::string msg);
    void debug(std::string parent, std::string msg);
    void error(std::string parent, std::string msg);
    void warning(std::string parent, std::string msg);
    void text(std::string tag, std::string parent, std::string msg);
private:
    std::fstream fs;
    std::string filename;
};

#endif // LOGGER_H
