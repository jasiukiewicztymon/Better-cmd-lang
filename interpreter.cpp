#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <map>
#include <iterator>
#include <sstream>
#include <Windows.h>

#include "interpreter.h"

void interpret(
    std::string& command, 
    std::map<std::string, std::string> &Val,
    std::map<std::string, std::vector<std::string>> &List, 
    std::map<std::string, std::string> &Function,
    std::string &path
) {
    std::vector<std::string> splitCommand;
    std::stringstream ss(command);
    std::string tempstr;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    while (ss >> tempstr) {
        splitCommand.emplace_back(tempstr);
    }

    try {
        if (splitCommand[0][0] == '$') {
            // variables
            if (splitCommand.size() == 1) {
                SetConsoleTextAttribute(hConsole, 12);
                std::cout << "Invalid argument\n";
                SetConsoleTextAttribute(hConsole, 15);
            }
            else if (splitCommand[1] == "=" && splitCommand.size() == 3) {
                // definition
                if (splitCommand[2][0] == '$' || splitCommand[2][0] == '#') {
                    if (splitCommand[2][0] == '$') {
                        bool ok = false;
                        for (auto j = Val.begin(); j != Val.end(); j++) {
                            if (j->first == splitCommand[2]) {
                                ok = true;
                            }
                        }
                        if (ok) {
                            Val.insert(Val.end(), std::pair<std::string, std::string>(splitCommand[0], splitCommand[2]));
                        }
                        else {
                            SetConsoleTextAttribute(hConsole, 12);
                            std::cout << "Invalid arguments\n";
                            SetConsoleTextAttribute(hConsole, 15);
                        }
                    }
                    else
                        Val.insert(Val.end(), std::pair<std::string, std::string>(splitCommand[0], splitCommand[2]));
                }
                else {
                    SetConsoleTextAttribute(hConsole, 12);
                    std::cout << "Invalid arguments\n";
                    SetConsoleTextAttribute(hConsole, 15);
                }
            }
            else {
                SetConsoleTextAttribute(hConsole, 12);
                std::cout << "Invalid arguments\n";
                SetConsoleTextAttribute(hConsole, 15);
            }
        }
        else if (splitCommand[0][0] == '@') {
            // lists
            std::map<std::string, std::vector<std::string>>::iterator it = List.find(splitCommand[0]);
            if (splitCommand.size() == 1) {
                SetConsoleTextAttribute(hConsole, 12);
                std::cout << "Invalid argument\n";
                SetConsoleTextAttribute(hConsole, 15);
            }
            else if (splitCommand[1] == "-add" && splitCommand.size() >= 3) {
                // addition
                try {
                    bool isok = true;
                    for (int i = 2; i < splitCommand.size(); i++) {
                        if (splitCommand[i][0] != '$' && splitCommand[i][0] != '#') {
                            isok = false;
                            break;
                        }
                        if (splitCommand[i][0] == '$') {
                            bool ok = false;
                            for (auto j = Val.begin(); j != Val.end(); j++) {
                                if (j->first == splitCommand[i]) {
                                    ok = true;
                                }
                            }
                            if (!ok) {
                                isok = false;
                                break;
                            }
                        }
                    }
                    if (isok) {
                        if (List.find(splitCommand[0]) != List.end()) {
                            for (int i = 2; i < splitCommand.size(); i++) {
                                List.find(splitCommand[0])->second.emplace_back(splitCommand[i]);
                            }
                        }
                        else {
                            std::vector<std::string> Vtemp;
                            for (int i = 2; i < splitCommand.size(); i++) {
                                Vtemp.emplace_back(splitCommand[i]);
                            }
                            List.insert(std::pair<std::string, std::vector<std::string>>(splitCommand[0], Vtemp));
                        }
                    }
                    else {
                        SetConsoleTextAttribute(hConsole, 12);
                        std::cout << "Invalid argument\n";
                        SetConsoleTextAttribute(hConsole, 15);
                    }
                }
                catch (...) {
                    SetConsoleTextAttribute(hConsole, 12);
                    std::cout << "Invalid list name\n";
                    SetConsoleTextAttribute(hConsole, 15);
                }
            }
            else if (splitCommand[1] == "-del" && splitCommand.size() == 3) {
                // suppresion
                if (splitCommand[2][0] == '$' || splitCommand[2][0] == '#') {
                    std::vector<std::string>::iterator tempindex = std::find(List.find(splitCommand[0])->second.begin(), List.find(splitCommand[0])->second.end(), splitCommand[2]);
                    if (tempindex != List.find(splitCommand[0])->second.end())
                        List.find(splitCommand[0])->second.erase(tempindex);
                }
                else {
                    SetConsoleTextAttribute(hConsole, 12);
                    std::cout << "Invalid arguments\n";
                    SetConsoleTextAttribute(hConsole, 15);
                }
            }
            else if (splitCommand[1] == "-list" && splitCommand.size() == 2) {
                auto ittemp = List.find(splitCommand[0]);
                if (ittemp != List.end()) {
                    for (int i = 0; i < ittemp->second.size(); i++) {
                        std::cout << List.find(splitCommand[0])->second[i] << ";";
                    }
                    std::cout << "\n";
                }
                else {
                    SetConsoleTextAttribute(hConsole, 12);
                    std::cout << "Invalid list name\n";
                    SetConsoleTextAttribute(hConsole, 15);
                }
            }
            else if (splitCommand[1] == "-size" && splitCommand.size() == 2) {
                auto ittemp = List.find(splitCommand[0]);
                if (ittemp != List.end()) {
                    std::cout << ittemp->second.size() << "\n";
                }
                else {
                    SetConsoleTextAttribute(hConsole, 12);
                    std::cout << "Invalid list name\n";
                    SetConsoleTextAttribute(hConsole, 15);
                }
            }
            else {
                SetConsoleTextAttribute(hConsole, 12);
                std::cout << "Invalid arguments\n";
                SetConsoleTextAttribute(hConsole, 15);
            }
        }
        else {
            // commands
            if (splitCommand[0] == "print" || splitCommand[0] == "echo") {
                std::string finalstr = "";
                try {
                    for (int i = 0; i < splitCommand.size(); i++) {
                        if (splitCommand[i][0] == '$') {
                            auto tempit = Val.find(splitCommand[i]);
                            if (tempit != Val.end())
                                finalstr += tempit->second + " ";
                            else
                                throw "Invalid variable";
                        }
                        else if (splitCommand[i][0] == '@' && i < (splitCommand.size() - 1)) {
                            auto tempit = List.find(splitCommand[i]);
                            if (tempit != List.end()) {
                                if (stoi(splitCommand[i + 1]) < tempit->second.size()) {
                                    finalstr += tempit->second[stoi(splitCommand[i + 1])] + " ";
                                }
                                else {
                                    throw "Invalid index in list";
                                }
                            }
                            else {
                                throw "Invalid list";
                            }
                        }
                        else if (splitCommand[i][0] == '#') {
                            finalstr += splitCommand[i].substr(1, splitCommand[i].size()) + " ";
                        }
                        else {
                            throw "Invalid argument";
                        }
                    }
                    std::cout << finalstr << "\n";
                }
                catch (std::string e) {
                    SetConsoleTextAttribute(hConsole, 12);
                    std::cout << e << "\n";
                    SetConsoleTextAttribute(hConsole, 15);
                }
            }
            else {
                SetConsoleTextAttribute(hConsole, 12);
                std::cout << "Invalid arguments\n";
                SetConsoleTextAttribute(hConsole, 15);
            }
        }
    }
    catch (...) {
        SetConsoleTextAttribute(hConsole, 12);
        std::cout << "Invalid arguments\n";
        SetConsoleTextAttribute(hConsole, 15);
    }
}
