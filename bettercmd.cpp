#include <iostream>
#include <map>
#include <vector>
#include <stdexcept>
#include <string>

#include "interpreter.h"

std::string command;
int exited;

std::map<std::string, int> IntVal;
std::map<std::string, double> DoubleVal;
std::map<std::string, bool> BoolVal;
std::map<std::string, char> CharVal;
std::map<std::string, std::string> StringVal;

std::map<std::string, std::vector<int>> IntList;
std::map<std::string, std::vector<double>> DoubleList;
std::map<std::string, std::vector<bool>> BoolList;
std::map<std::string, std::vector<char>> CharList;
std::map<std::string, std::vector<std::string>> StringList;

int main(int argc, char* argv[]) {
    do {
        std::getline(std::cin, command);
        interpret(command, IntVal, DoubleVal, BoolVal, CharVal, StringVal, IntList, DoubleList, BoolList, CharList, StringList);
    } while (command != "exit");
}
