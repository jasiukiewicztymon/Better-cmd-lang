#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <map>

#include "interpreter.h"

void interpret(
    std::string& command, 
    std::map<std::string, int> &IntVal,
    std::map<std::string, double> &DoubleVal,
    std::map<std::string, bool> &BoolVal,
    std::map<std::string, char> &CharVal,
    std::map<std::string, std::string> &StringVal,
    std::map<std::string, std::vector<int>> &IntList,
    std::map<std::string, std::vector<double>> &DoubleList,
    std::map<std::string, std::vector<bool>> &BoolList,
    std::map<std::string, std::vector<char>> &CharList,
    std::map<std::string, std::vector<std::string>> &StringList
) {
    std::vector<int> iTemp;
    std::vector<double> dblTemp;
    std::vector<bool> isTemp;
    std::vector<char> cTemp;
    std::vector<std::string> strTemp;

    std::string directive;
    std::vector<std::string> arguments;
    char delim = ' ';
    int start = 0;
    int end = -1;
    end = command.find(delim);

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
        //if (arguments.size() != )
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
    Default error printing
    */
    else {

    }
}
