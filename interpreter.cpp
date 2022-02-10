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
    std::map<std::string, std::string>& Val,
    std::map<std::string, std::vector<std::string>>& List,
    std::string& path,
    int& userColor,
    int& pathColor
) {


    std::vector<std::string> splitCommand;
    std::stringstream ss(command);
    std::string tempstr;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    while (ss >> tempstr) {
        splitCommand.emplace_back(tempstr);
    }

    try {
        /*
        Variables:

        Example of variable definition:
        $variable_name = "text"
        $variable_name = $other_variable
        $variable_name = @list "1"
        $variable_name = @list $index

        */
        if (splitCommand[0][0] == '$') {
            if (splitCommand.size() == 1) {
                errorMsg("Invalid argument\n");
            }
            else if (splitCommand[1] == "=") {
                auto valit = Val.find(splitCommand[0]);
                if (valit == Val.end()) {
                    if ((splitCommand[2][0] == '$' && splitCommand.size() == 3) || (splitCommand[2][0] == '"' && splitCommand[splitCommand.size() - 1][splitCommand[splitCommand.size() - 1].size() - 1] == '"') || (splitCommand[2][0] == '@' && splitCommand.size() == 3)) {
                        /*
                        Variable to variable
                        */
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
                        /*
                        List to variable
                        */
                        else if (splitCommand[2][0] == '@') {
                            auto it = List.find(splitCommand[2]);
                            if (it == List.end()) {
                                errorMsg("Invalid list\n");
                            }
                            else {
                                int pos;
                                if (splitCommand[3][0] == '$') {
                                    std::string valcontent;
                                    auto valpos = Val.find(splitCommand[3]);
                                    if (valpos == Val.end()) {
                                        errorMsg("Invalid variable\n");
                                    }
                                    else {
                                        valcontent = valpos->second;
                                        while (valcontent[0] == '$') {
                                            valpos = Val.find(valcontent);
                                            valcontent = valpos->second;
                                        }
                                        pos = stoi(valcontent);
                                        if (pos < it->second.size()) {
                                            Val.insert(Val.end(), std::pair<std::string, std::string>(splitCommand[0], it->second[pos]));
                                        }
                                        else {
                                            errorMsg("Invalid index\n");
                                        }
                                    }
                                }
                                else {
                                    pos = stoi(splitCommand[3].substr(1, splitCommand[3].size() - 1));
                                    if (pos < it->second.size()) {
                                        Val.insert(Val.end(), std::pair<std::string, std::string>(splitCommand[0], it->second[pos]));
                                    }
                                    else {
                                        errorMsg("Invalid index\n");
                                    }
                                }
                            }
                        }
                        /*
                        String to variable
                        */
                        else {
                            int counting = 0;
                            for (int k = splitCommand[splitCommand.size() - 1].size() - 2; k == '\\' && k >= 0; k--) {
                                counting++;
                            }
                            if (counting % 2 == 0) {
                                std::string out = "", endout = "";
                                for (int k = 2; k < splitCommand.size(); k++) {
                                    out += splitCommand[k] + " ";
                                }
                                out = out.substr(1, out.size() - 2);

                                int start = 0;
                                std::string del = "\\\"";
                                int end = out.find(del);
                                while (end != -1) {
                                    endout += out.substr(start, end - start) + "\"";
                                    start = end + del.size();
                                    end = out.find(del, start);
                                }
                                endout += out.substr(start, end - start);

                                start = 0;
                                del = "\\\\";
                                out = "";
                                end = endout.find(del);
                                while (end != -1) {
                                    out += endout.substr(start, end - start) + "\\";
                                    start = end + del.size();
                                    end = endout.find(del, start);
                                }
                                out += endout.substr(start, end - start);

                                Val.insert(Val.end(), std::pair<std::string, std::string>(splitCommand[0], out.substr(0, out.size() - 1)));
                            }
                            else {
                                errorMsg("Invalid string\n");
                            }
                        }
                    }
                    else {
                        errorMsg("Invalid argument\n");
                    }
                }
                /*
                Redefinition of a variable
                */
                else {
                    if ((splitCommand[2][0] == '@' && splitCommand.size() == 4) || (splitCommand[2][0] == '$' && splitCommand.size() == 3) || (splitCommand.size() >= 3 && splitCommand[2][0] == '"' && splitCommand[splitCommand.size() - 1][splitCommand[splitCommand.size() - 1].size() - 1] == '"')) {
                        /*
                        Variable to variable redefinition
                        */
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
                        /*
                        List to variable redefinition
                        */
                        else if (splitCommand[2][0] == '@') {
                            auto it = List.find(splitCommand[2]);
                            if (it == List.end()) {
                                errorMsg("Invalid list\n");
                            }
                            else {
                                int pos;
                                if (splitCommand[3][0] == '$') {
                                    std::string valcontent;
                                    auto valpos = Val.find(splitCommand[3]);
                                    if (valpos == Val.end()) {
                                        errorMsg("Invalid variable\n");
                                    }
                                    else {
                                        valcontent = valpos->second;
                                        while (valcontent[0] == '$') {
                                            valpos = Val.find(valcontent);
                                            valcontent = valpos->second;
                                        }
                                        pos = stoi(valcontent);
                                        if (pos < it->second.size()) {
                                            valit->second = it->second[pos];
                                        }
                                        else {
                                            errorMsg("Invalid index\n");
                                        }
                                    }
                                }
                                else {
                                    pos = stoi(splitCommand[3].substr(1, splitCommand[3].size() - 1));
                                    if (pos < it->second.size()) {
                                        valit->second = it->second[pos];
                                    }
                                    else {
                                        errorMsg("Invalid index\n");
                                    }
                                }
                            }
                        }
                        /*
                        Text to variable redefinition
                        */
                        else {
                            int counting = 0;
                            for (int k = splitCommand[splitCommand.size() - 1].size() - 2; k == '\\' && k >= 0; k--) {
                                counting++;
                            }
                            if (counting % 2 == 0) {
                                std::string out = "", endout = "";
                                for (int k = 2; k < splitCommand.size(); k++) {
                                    out += splitCommand[k];
                                }
                                out = out.substr(1, out.size() - 2);

                                int start = 0;
                                std::string del = "\\\"";
                                int end = out.find(del);
                                while (end != -1) {
                                    endout += out.substr(start, end - start) + "\"";
                                    start = end + del.size();
                                    end = out.find(del, start);
                                }
                                endout += out.substr(start, end - start);

                                start = 0;
                                del = "\\\\";
                                out = "";
                                end = endout.find(del);
                                while (end != -1) {
                                    out += endout.substr(start, end - start) + "\\";
                                    start = end + del.size();
                                    end = endout.find(del, start);
                                }
                                out += endout.substr(start, end - start);

                                valit->second = out;
                            }
                            else {
                                errorMsg("Invalid string\n");
                            }
                        }
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
        /*
        List:

        Example of list definition:
        @list -add "possible text" $variable ...
        @list -del "content"
        @list -del $variable

        */
        else if (splitCommand[0][0] == '@') {
            std::map<std::string, std::vector<std::string>>::iterator it = List.find(splitCommand[0]);
            if (splitCommand.size() == 1) {
                errorMsg("Invalid argument\n");
            }
            else if (splitCommand[1] == "-add" && splitCommand.size() >= 3) {
                if (it == List.end()) {

                }
                else {

                }
            }
            else if (splitCommand[1] == "-del" && splitCommand.size() == 3) {
                auto vfind = std::find(it->second.begin(), it->second.end(), splitCommand[2]);
                it->second.erase(vfind);
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
            if (splitCommand[0] == "print" || splitCommand[0] == "echo") {
            
            }
            else if (splitCommand[0] == "clear" || splitCommand[0] == "cls") {
                system("cls");
            }
            else if (splitCommand[0] == "title") {
                if (splitCommand[1] == "-set") {
                    
                }
                else if (splitCommand[1] == "-reset" && splitCommand.size() == 2) {
                    SetConsoleTitleW(L"Better CMD");
                }
                else
                    errorMsg("Invalid syntax\n");
            }
            else if (splitCommand[0] == "color") {
                
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
