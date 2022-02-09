#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <map>
#include <iterator>
#include <sstream>
#include <Windows.h>

#include "bcin.h"
#include "interpreter.h"

void errorMsg(std::string alert) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 12);
    std::cout << alert;
    SetConsoleTextAttribute(hConsole, 15);
}

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
                errorMsg("Invalid argument\n");
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
                            errorMsg("Invalid argument\n");
                        }
                    }
                    else
                        Val.insert(Val.end(), std::pair<std::string, std::string>(splitCommand[0], splitCommand[2]));
                }
                else {
                    errorMsg("Invalid argument\n");
                }
            }
            else {
                errorMsg("Invalid argument\n");
            }
        }
        else if (splitCommand[0][0] == '@') {
            // lists
            std::map<std::string, std::vector<std::string>>::iterator it = List.find(splitCommand[0]);
            if (splitCommand.size() == 1) {
                errorMsg("Invalid argument\n");
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
                        errorMsg("Invalid argument\n");
                    }
                }
                catch (...) {
                    errorMsg("Invalid list name\n");
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
                    errorMsg("Invalid argument\n");
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
                    errorMsg("Invalid list name\n");
                }
            }
            else if (splitCommand[1] == "-size" && splitCommand.size() == 2) {
                auto ittemp = List.find(splitCommand[0]);
                if (ittemp != List.end()) {
                    std::cout << ittemp->second.size() << "\n";
                }
                else {
                    errorMsg("Invalid list name\n");
                }
            }
            else {
                errorMsg("Invalid argument\n");
            }
        }
        else {
            // commands
            if (splitCommand[0] == "print" || splitCommand[0] == "echo") {
                std::string finalstr = "";
                if (splitCommand.size() == 1) {
                    errorMsg("Invalid argument\n");
                }
                else {
                    bool validation = true;
                    for (int i = 0; i < splitCommand.size(); i++) {
                        if (splitCommand[i][0] == '$') {
                            auto tempit = Val.find(splitCommand[i]);
                            if (tempit != Val.end())
                                finalstr += tempit->second + " ";
                            else {
                                errorMsg("Invalid variable\n");
                                validation = false;
                                break;
                            }
                        }
                        else if (splitCommand[i][0] == '@' && i < (splitCommand.size() - 1)) {
                            auto tempit = List.find(splitCommand[i]);
                            if (tempit != List.end()) {
                                if (stoi(splitCommand[i + 1].substr(1, splitCommand[i + 1].size())) < tempit->second.size()) {
                                    if (tempit->second[stoi(splitCommand[i + 1])][0] == '#') {
                                        finalstr += splitCommand[i].substr(1, splitCommand[i].size()) + " ";
                                    }
                                    else if (tempit->second[stoi(splitCommand[i + 1])][0] == '$') {
                                        auto tempit2 = Val.find(splitCommand[i]);
                                        if (tempit2 != Val.end())
                                            finalstr += tempit2->second + " ";
                                        else {
                                            errorMsg("Invalid variable\n");
                                            validation = false;
                                            break;
                                        }
                                    }
                                    else {
                                        errorMsg("Invalid object\n");
                                        validation = false;
                                        break;
                                    }
                                }
                                else {
                                    errorMsg("Invalid index in list\n");
                                    validation = false;
                                    break;
                                }
                            }
                            else {
                                errorMsg("Invalid list\n");
                                validation = false;
                                break;
                            }
                        }
                        else if (splitCommand[i][0] == '#') {
                            finalstr += splitCommand[i].substr(1, splitCommand[i].size()) + " ";
                        }
                        else {
                            errorMsg("Invalid argument\n");
                            validation = false;
                            break;
                        }
                    }
                    if (validation)
                        std::cout << finalstr << "\n";
                }
            }
            else {
                errorMsg("Invalid argument\n");
            }
        }
    }
    catch (...) {
        errorMsg("Invalid argument\n");
    }
}
