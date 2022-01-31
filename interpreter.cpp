#include <string>
#include <iostream>
#include <vector>

#include "interpreter.h"

void interpret(std::string &command, std::vector<int> &IntVal, std::vector<double> &DoubleVal, std::vector<bool> &BoolVal, std::vector<char> &CharVal, std::vector<std::string> &StringVal, std::vector<std::vector<int>> &IntList, std::vector<std::vector<double>> &DoubleList, std::vector<std::vector<bool>> &BoolList, std::vector<std::vector<char>> &CharList, std::vector<std::vector<std::string>> &StringList) {
    std::string directive;
    std::vector<std::string> arguments;
    std::getline(std::cin, command);
    char delim = ' ';
    int start = 0;
    int end = command.find(delim);

    directive = command.substr(start, end - start);
    start = end + 1;
    end = command.find(delim, start);

    while (end != -1) {
        arguments.emplace_back(command.substr(start, end - start));
        start = end + 1;
        end = command.find(delim, start);
    }
    arguments.emplace_back(command.substr(start, end - start));

    /*
    Definition of variable
    */
    if (directive == "val") {

    }
    /*
    Definition of array
    */
    else if (directive == "list") {

    }
    /*
    Creation of statement
    */
    else if (directive == "statement") {

    }
    /*
    Creation of loop
    */
    else if (directive == "loop") {

    }
    /*
    Creation of function
    */
    else if (directive == "function") {

    }
    /*
    Print function
    */
    else if (directive == "print") {

    }
    /*
    Input function
    */
    else if (directive == "get") {

    }
    /*
    Extention managing
    */
    else if (directive == "ext") {

    }
    /*
    CMD call
    */
    else if (directive == "cmd") {

    }
    /*
    Exit command
    */
    else if (directive == "exit") {
        system("exit");
    }
    /*
    Default error printing
    */
    else {

    }
}