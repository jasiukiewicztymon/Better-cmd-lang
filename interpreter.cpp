#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <map>
#include <iterator>
#include <sstream>
#include <Windows.h>
#include <cassert>
#include <cstring>

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
    std::string &path,
    int &userColor,
    int &pathColor
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
                auto valit = Val.find(splitCommand[0]);
                if (valit == Val.end()) {
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
                    if (splitCommand[2][0] == '$' || splitCommand[2][0] == '#') {
                        if (splitCommand[2][0] == '$') {
                            bool ok = false;
                            for (auto j = Val.begin(); j != Val.end(); j++) {
                                if (j->first == splitCommand[2]) {
                                    ok = true;
                                }
                            }
                            if (ok) {
                                valit->second = splitCommand[2];
                            }
                            else {
                                errorMsg("Invalid argument\n");
                            }
                        }
                        else
                            valit->second = splitCommand[2];
                    }
                    else {
                        errorMsg("Invalid argument\n");
                    }
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
                    for (int i = 1; i < splitCommand.size(); i++) {
                        if (splitCommand[i][0] == '$') {
                            auto tempit = Val.find(splitCommand[i]);
                            if (tempit != Val.end()) {
                                if (tempit->second[0] == '#') {
                                    finalstr += tempit->second.substr(1, tempit->second.size()) + " ";
                                }
                                else {
                                    tempit->second;
                                    while (tempit->second[0] == '$') {
                                        std::string itname = tempit->second;
                                        tempit = Val.find(itname);
                                    }
                                    finalstr += tempit->second.substr(1, tempit->second.size()) + " ";
                                }
                            }
                            else {
                                errorMsg("Invalid variable\n");
                                validation = false;
                                break;
                            }
                        }
                        else if (splitCommand[i][0] == '@' && i < (splitCommand.size() - 1)) {
                            auto tempit = List.find(splitCommand[i]);
                            if (tempit != List.end()) {
                                if (splitCommand[i + 1][0] == '#') {
                                    if (stoi(splitCommand[i + 1].substr(1, splitCommand[i + 1].size())) < tempit->second.size()) {
                                        finalstr += tempit->second[stoi(splitCommand[i + 1].substr(1, splitCommand[i + 1].size()))].substr(1, tempit->second[stoi(splitCommand[i + 1].substr(1, splitCommand[i + 1].size()))].size()) + " ";
                                    }
                                    else {
                                        errorMsg("Invalid index in list\n");
                                        validation = false;
                                        break;
                                    }
                                }
                                else if (splitCommand[i + 1][0] == '$') {
                                    auto tempit2 = Val.find(splitCommand[i + 1]);
                                    int itindex;
                                    if (tempit2 != Val.end()) {
                                        if (tempit2->second[0] == '#') {
                                            itindex = stoi(tempit2->second.substr(1, tempit2->second.size()));
                                        }
                                        else {
                                            tempit2->second;
                                            while (tempit2->second[0] == '$') {
                                                std::string itname = tempit2->second;
                                                tempit2 = Val.find(itname);
                                            }
                                            itindex = stoi(tempit2->second.substr(1, tempit2->second.size()));
                                        }

                                        finalstr += tempit->second[itindex].substr(1, tempit->second[itindex].size()) + " ";
                                    }
                                    else {
                                        errorMsg("Invalid variable\n");
                                        validation = false;
                                        break;
                                    }
                                }
                                else {
                                    errorMsg("Invalid index in list\n");
                                    validation = false;
                                    break;
                                }
                                ++i;
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
            else if (splitCommand[0] == "clear" || splitCommand[0] == "cls") {
                system("cls");
            }
            else if (splitCommand[0] == "title") {
                if (splitCommand[1] == "-set") {
                    std::string title;
                    if ((splitCommand[2][0] == '$' || splitCommand[2][0] == '#') && splitCommand.size() == 3) {
                        if (splitCommand[2][0] == '$') {
                            auto tempit = Val.find(splitCommand[2]);
                            if (tempit != Val.end()) {
                                if (tempit->second[0] == '#') {
                                    title = tempit->second.substr(1, tempit->second.size());
                                    system(("title " + title).c_str());
                                }
                                else {
                                    tempit->second;
                                    while (tempit->second[0] == '$') {
                                        std::string itname = tempit->second;
                                        tempit = Val.find(itname);
                                    }
                                    title = tempit->second.substr(1, tempit->second.size());
                                    system(("title " + title).c_str());
                                }
                            }
                            else {
                                errorMsg("Invalid variable\n");
                            }
                        }
                        else {
                            system(("title " + splitCommand[2].substr(1, splitCommand[2].size())).c_str());
                        }
                    }
                    else if (splitCommand[2][0] == '@' && splitCommand.size() == 4) {
                        bool isok = true;
                        auto tempit = List.find(splitCommand[2]);
                        if (tempit != List.end()) {
                            if (splitCommand[3][0] == '#') {
                                int position = stoi(splitCommand[3].substr(1, splitCommand[3].size()));
                                if (position < tempit->second.size()) {
                                    title = tempit->second[position];
                                    title = title.substr(1, title.size());
                                }
                                else {
                                    errorMsg("Invalid index in list\n");
                                    isok = false;
                                }
                            }
                            //test
                            else if (splitCommand[3][0] == '$') {
                                auto tempit2 = Val.find(splitCommand[3]);
                                int position;
                                if (tempit2 != Val.end()) {
                                    if (tempit2->second[0] == '#') {
                                        position = stoi(tempit2->second.substr(1, tempit2->second.size()));
                                    }
                                    else {
                                        tempit2->second;
                                        while (tempit2->second[0] == '$') {
                                            std::string itname = tempit2->second;
                                            tempit2 = Val.find(itname);
                                        }
                                        position = stoi(tempit2->second.substr(1, tempit2->second.size()));
                                    }

                                    title = tempit->second[position];
                                    title = title.substr(1, title.size());
                                }
                                else {
                                    errorMsg("Invalid variable\n");
                                    isok = false;
                                }
                            }
                            else {
                                errorMsg("Invalid index in list\n");
                                isok = false;
                            }
                        }
                        else {
                            errorMsg("Invalid list\n");
                            isok = false;
                        }

                        if (isok)
                            system(("title " + title).c_str());
                    }
                    else {
                        errorMsg("Invalid syntax\n");
                    }
                }
                else if (splitCommand[1] == "-reset" && splitCommand.size() == 2) {
                    SetConsoleTitleW(L"Better CMD");
                }
                else
                    errorMsg("Invalid syntax\n");
            }
            else if (splitCommand[0] == "color") {
                if (splitCommand.size() != 3) {
                    if (splitCommand[1] == "-name") {
                        if (stoi(splitCommand[2].substr(1, splitCommand[2].size())) >= 0 && stoi(splitCommand[2].substr(1, splitCommand[2].size())) <= 15) {
                            userColor = stoi(splitCommand[2].substr(1, splitCommand[2].size()));
                        }
                        else {
                            errorMsg("Invalid color\n");
                        }
                    }
                    else if (splitCommand[1] == "-path") {
                        if (stoi(splitCommand[2].substr(1, splitCommand[2].size())) >= 0 && stoi(splitCommand[2].substr(1, splitCommand[2].size())) <= 15) {
                            pathColor = stoi(splitCommand[2].substr(1, splitCommand[2].size()));
                        }
                        else {
                            errorMsg("Invalid color\n");
                        }
                    }
                    else 
                        errorMsg("Invalid argument\n");
                }
                else
                    errorMsg("Invalid argument\n");
            }
            else if (splitCommand[0] == "exit") {}
            else {
                errorMsg("Invalid argument\n");
            }
        }
    }
    catch (...) {
        errorMsg("Invalid argument\n");
    }
}
