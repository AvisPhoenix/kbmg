// Copyleft 2024 Avis Phoenix
// SPDX-License-Identifier: GPL-3.0-only

#include "procrunner.h"

void ProcRunner::resetOutput()
{
    outputStr = "";
    errorStr = "";
    status_code = -1;
}

ProcRunner::ProcRunner(string cmd) {
    command = cmd;
    resetOutput();
}

void ProcRunner::readErrors(redi::ipstream &process, bool finished[], string &line) {
    if (!finished[0])
    {
        while (std::getline(process.err(), line))
            errorStr += line + "\n";
        if (process.eof())
        {
            finished[0] = true;
            if (!finished[1])
                process.clear();
        }
    }
}

void ProcRunner::readOutput(redi::ipstream &process, bool finished[], string &line) {
    if (!finished[1])
    {
        while (std::getline(process.out(), line))
            outputStr += line + "\n";
        if (process.eof())
        {
            finished[1] = true;
            if (!finished[0])
                process.clear();
        }
    }
}

void ProcRunner::save2RAM(bool finished[], redi::ipstream &process, string &line)
{
    while (!finished[0] || !finished[1]) {
        readErrors(process, finished, line);

        readOutput(process, finished, line);
    }
}

void ProcRunner::saveCommandOutput(redi::ipstream &process) {
    string line;
    bool finished[2] = { false, false };
    save2RAM(finished, process, line);
    process.close();
    if (process.rdbuf()->exited()) {
        status_code = process.rdbuf()->status();
    }
}

redi::ipstream ProcRunner::buildProcedure(const std::vector<string> *argv){
    const redi::pstreams::pmode mode = redi::pstreams::pstdout|redi::pstreams::pstderr;
    if (argv != nullptr){
        std::vector<string> cmd({command});
        cmd.insert(cmd.end(), argv->begin(), argv->end());
        return redi::ipstream(cmd, mode);
    }else{
        return redi::ipstream(command, mode);
    }
}

int ProcRunner::run(const std::vector<string> *argv) {
    resetOutput();
    redi::ipstream process = buildProcedure(argv);
    saveCommandOutput(process);
    return status();
}






