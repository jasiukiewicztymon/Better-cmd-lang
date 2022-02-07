#include "bcin.h"
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <cstddef>

#include <conio.h>
#include <stdio.h>
#include <Windows.h>

void bcin(std::string& output, std::string prefix, std::map<std::string, std::string> Val, std::map<std::string, std::string> Function, std::map<std::string, std::vector<std::string>> List, std::vector<std::string> Keywords) {
    char ch = ' ';

    std::vector<std::string> ValNames;
    std::vector<std::string> ValFunction;
    std::vector<std::string> ValList;

    for (std::map<std::string, std::string>::iterator it = Val.begin(); it != Val.end(); it++) {
        ValNames.emplace_back(it->first);
    }
    for (std::map<std::string, std::string>::iterator it = Function.begin(); it != Function.end(); it++) {
        ValFunction.emplace_back(it->first);
    }
    for (std::map<std::string, std::vector<std::string>>::iterator it = List.begin(); it != List.end(); it++) {
        ValList.emplace_back(it->first);
    }

    std::cout << prefix;

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO cbsi;
    GetConsoleScreenBufferInfo(h, &cbsi);
    COORD pos = cbsi.dwCursorPosition;

    COORD screen;

    std::string input = " ", newinput;
    std::vector<std::string> splitInput;
    int X = pos.X, Y = pos.Y, minX = X, maxX = 0, index = 1, wordindex = 0;
    screen.Y = Y;
    bool backspace = false, del = false, left = false, right = false, tab = false;

    do {
        int key;
        if (_kbhit()) {
            key = _getch();
            if (key == 224) {
                do {
                    key = _getch();
                } while (key == 224);
                if (key == 75) {
                    left = true;
                }
                else if (key == 77) {
                    right = true;
                }
                else if (key == 83) {
                    del = true;
                }
            }
            else {
                ch = key;
                if (ch == 8) {
                    backspace = true;
                }
                else if (ch == 9) {
                    tab = true;
                }
                else {
                    if (ch != 13) {
                        std::string tempch = "";
                        tempch += ch;
                        input.insert(index, tempch);
                        index++;
                        maxX++;
                    }
                }
            }
            screen.X = minX;
            SetConsoleCursorPosition(h, screen);

            for (int i = 0; i < input.size() * 10; i++) {
                std::cout << " ";
            }

            if (backspace) {
                if (index >= minX) {
                    input.erase(index - 1, 1);
                    index--;
                    maxX--;
                }
                backspace = false;
            }
            else if (del) {
                if (index < maxX + 1) {
                    input.erase(index, 1);
                    maxX--;
                }
                del = false;
            }
            else if (left) {
                if (index >= minX) {
                    index--;
                }
                left = false;
            }
            else if (right) {
                if (index < maxX + 1) {
                    index++;
                }
                right = false;
            }

            screen.X = minX;
            SetConsoleCursorPosition(h, screen);

            std::vector<std::string> labels;

            std::string del = " ";
            int start = 0;
            int end = input.find(del);
            while (end != -1) {
                labels.emplace_back(input.substr(start, end - start));
                start = end + del.size();
                end = input.find(del, start);
            }
            labels.emplace_back(input.substr(start, end - start));

            std::vector<int> temp;
            std::vector<std::vector<int>> sizesref;

            temp.emplace_back(0);
            temp.emplace_back(labels[0].size());
            sizesref.emplace_back(temp);
            temp.clear();

            if (labels.size() > 1)
                for (int i = 1; i < labels.size() - 1; i++) {
                    temp.emplace_back(sizesref[i - 1][1] + 1);
                    temp.emplace_back(sizesref[i - 1][1] + 1 + labels[i].size());
                    sizesref.emplace_back(temp);
                    temp.clear();
                }

            temp.emplace_back(sizesref[sizesref.size() - 1][1] + 1);
            temp.emplace_back(input.size());
            sizesref.emplace_back(temp);
            temp.clear();

            int remindex = 0;

            for (int i = 0; i < labels.size(); i++) {
                bool is = false;

                if (labels[i] == "=") {
                    std::cout << labels[i] << " ";
                    continue;
                }
                else if (labels[i] == "==") {
                    std::cout << labels[i] << " ";
                    continue;
                }
                else if (labels[i] == "<") {
                    std::cout << labels[i] << " ";
                    continue;
                }
                else if (labels[i] == ">") {
                    std::cout << labels[i] << " ";
                    continue;
                }
                else if (labels[i] == "<=") {
                    std::cout << labels[i] << " ";
                    continue;
                }
                else if (labels[i] == ">=") {
                    std::cout << labels[i] << " ";
                    continue;
                }
                else if (labels[i] == "!=") {
                    std::cout << labels[i] << " ";
                    continue;
                }
                else if (labels[i][0] == '#') {
                    std::cout << labels[i] << " ";
                    continue;
                }
                if (!tab || (index < sizesref[i][0] || index > sizesref[i][1])) {
                    if (labels[i][0] == '$') {
                        for (int k = 0; k < ValNames.size(); k++) {
                            if (labels[i] == ValNames[k])
                                is = true;
                        }

                        if (is) {
                            SetConsoleTextAttribute(h, 9);
                            std::cout << labels[i];
                            SetConsoleTextAttribute(h, 15);
                            is = false;
                        }
                        else {
                            SetConsoleTextAttribute(h, 12);
                            std::cout << labels[i];
                            SetConsoleTextAttribute(h, 15);
                        }
                    }
                    else if (labels[i][0] == '@') {
                        for (int k = 0; k < ValList.size(); k++) {
                            if (labels[i] == ValList[k])
                                is = true;
                        }

                        if (is) {
                            SetConsoleTextAttribute(h, 9);
                            std::cout << labels[i];
                            SetConsoleTextAttribute(h, 15);
                            is = false;
                        }
                        else {
                            SetConsoleTextAttribute(h, 12);
                            std::cout << labels[i];
                            SetConsoleTextAttribute(h, 15);
                        }
                    }
                    else {
                        for (int k = 0; k < ValFunction.size(); k++) {
                            if (labels[i] == ValFunction[k])
                                is = true;
                        }
                        for (int k = 0; k < Keywords.size(); k++) {
                            if (labels[i] == Keywords[k])
                                is = true;
                        }

                        if (is) {
                            SetConsoleTextAttribute(h, 10);
                            std::cout << labels[i];
                            SetConsoleTextAttribute(h, 15);
                            is = false;
                        }
                        else {
                            SetConsoleTextAttribute(h, 12);
                            std::cout << labels[i];
                            SetConsoleTextAttribute(h, 15);
                        }
                    }
                }

                if (i > 0 && (index >= sizesref[i][0] && index <= sizesref[i][1])) {
                    if (labels[i][0] == '#') {
                        tab = false;
                        continue;
                    }
                    if (labels[i][0] == '$') {
                        if (!tab) {
                            //Variables
                            SetConsoleTextAttribute(h, 8);
                            for (int j = 0; j < ValNames.size(); j++) {
                                if (ValNames[j].rfind(labels[i], 0) == 0) {
                                    std::cout << ValNames[j].substr(labels[i].size(), ValNames[j].size());
                                    break;
                                }
                            }
                            SetConsoleTextAttribute(h, 15);
                        }
                        else {
                            for (int j = 0; j < ValNames.size(); j++) {
                                if (ValNames[j].rfind(labels[i], 0) == 0) {
                                    SetConsoleTextAttribute(h, 9);
                                    input.insert(sizesref[i][1], ValNames[j].substr(labels[i].size(), ValNames[j].size()));
                                    std::cout << labels[i];
                                    std::cout << ValNames[j].substr(labels[i].size(), ValNames[j].size());
                                    index += ValNames[j].substr(labels[i].size(), ValNames[j].size()).size() + labels[i].size() - (index - sizesref[i][0]);
                                    maxX += ValNames[j].substr(labels[i].size(), ValNames[j].size()).size();
                                    tab = false;
                                    SetConsoleTextAttribute(h, 15);
                                    break;
                                }
                            }
                        }
                    }
                    else if (labels[i][0] == '@') {
                        if (!tab) {
                            //List
                            SetConsoleTextAttribute(h, 8);
                            for (int j = 0; j < ValList.size(); j++) {
                                if (ValList[j].rfind(labels[i], 0) == 0) {
                                    std::cout << ValList[j].substr(labels[i].size(), ValList[j].size());
                                    break;
                                }
                            }
                            SetConsoleTextAttribute(h, 15);
                        }
                        else {
                            for (int j = 0; j < ValList.size(); j++) {
                                if (ValList[j].rfind(labels[i], 0) == 0) {
                                    SetConsoleTextAttribute(h, 9);
                                    input.insert(sizesref[i][1], ValList[j].substr(labels[i].size(), ValList[j].size()));
                                    std::cout << labels[i];
                                    std::cout << ValList[j].substr(labels[i].size(), ValList[j].size());
                                    index += ValList[j].substr(labels[i].size(), ValList[j].size()).size() + labels[i].size() - (index - sizesref[i][0]);
                                    maxX += ValList[j].substr(labels[i].size(), ValList[j].size()).size();
                                    tab = false;
                                    SetConsoleTextAttribute(h, 15);
                                    break;
                                }
                            }
                        }
                    }
                    else {
                        if (!tab) {
                            //Keywords
                            SetConsoleTextAttribute(h, 8);
                            for (int j = 0; j < Keywords.size(); j++) {
                                if (Keywords[j].rfind(labels[i], 0) == 0) {
                                    std::cout << Keywords[j].substr(labels[i].size(), Keywords[j].size());
                                    break;
                                }
                            }
                            SetConsoleTextAttribute(h, 15);
                        }
                        else {
                            for (int j = 0; j < Keywords.size(); j++) {
                                if (Keywords[j].rfind(labels[i], 0) == 0) {
                                    SetConsoleTextAttribute(h, 10);
                                    input.insert(sizesref[i][1], Keywords[j].substr(labels[i].size(), Keywords[j].size()));
                                    std::cout << labels[i];
                                    std::cout << Keywords[j].substr(labels[i].size(), Keywords[j].size());
                                    index += Keywords[j].substr(labels[i].size(), Keywords[j].size()).size() + labels[i].size() - (index - sizesref[i][0]);
                                    maxX += Keywords[j].substr(labels[i].size(), Keywords[j].size()).size();
                                    tab = false;
                                    SetConsoleTextAttribute(h, 15);
                                    break;
                                }
                            }
                        }
                    }
                }
                std::cout << " ";
            }

            if (tab)
                tab = false;

            screen.X = index + prefix.size();
            SetConsoleCursorPosition(h, screen);
            sizesref.clear();
        }
        splitInput.clear();
    } while (ch != 13);
    output = input.substr(1, input.size());
    std::cout << "\n";
}
