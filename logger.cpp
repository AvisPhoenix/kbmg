// Copyleft 2024 Avis Phoenix
// SPDX-License-Identifier: GPL-3.0-only

#include "logger.h"

Logger::Logger(std::string filename) {
    this->filename = filename;
    fs.open (filename, std::fstream::in | std::fstream::app);
    fs.close();
}

void Logger::info(std::string parent, std::string msg) {
    text("INFO", parent, msg);
}

void Logger::debug(std::string parent, std::string msg) {
    text("DEBUG", parent, msg);
}

void Logger::error(std::string parent, std::string msg) {
    text("ERROR", parent, msg);
}

void Logger::warning(std::string parent, std::string msg) {
    text("WARNING", parent, msg);
}

void Logger::text(std::string tag, std::string parent, std::string msg) {
    fs.open (filename, std::fstream::in | std::fstream::app);
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    char *t = std::ctime(&now_time);
    t[24] = '\0';
    fs << "[" << t<< "]["<< tag << "][" << parent << "]: " << msg << std::endl;
    fs.close();
}
