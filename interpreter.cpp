#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <map>
#include <iterator>

#include "interpreter.h"

void interpret(
    std::string& command, 
    std::map<std::string, std::string> &Val,
    std::map<std::string, std::vector<std::string>> &List
) {
    std::vector<std::string> Temp;

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
    for path use "path"
    */

    /*
    Transform variable to the     
    */
    for (int i = 0; i < arguments.size(); i++) {
        if (arguments[i][0] == '%' && arguments[i][arguments.size() - 1] == '%') {
            std::string strfind = arguments[i].substr(1, arguments.size() - 1);
            std::map<std::string, std::string>::iterator it;
            it = Val.find(strfind);
            if (it != Val.end())
                arguments[i] = Val.find(strfind)->second;
        }
    }

    for (int i = 0; i < arguments.size() - 1; i++) {
        if (arguments[i][0] == '%' && arguments[i][arguments.size() - 1] == '%' && arguments[i + 1][0] == '[' && arguments[i + 1][arguments.size() - 1] == ']') {
            std::string strfind = arguments[i].substr(1, arguments.size() - 1);
            int index = stoi(arguments[i + 1].substr(1, arguments.size() - 1));
            std::map<std::string, std::vector<std::string>>::iterator it;
            it = List.find(strfind);
            if (it != List.end()) {
                try {
                    arguments[i] = Val.find(strfind)->second[index];
                }
                catch (...) {
                    //Error message
                }
            }
        }
    }

    /*
    Definition of variable
    */
    if (directive == "val") {
        if (arguments.size() > 2) {
            for (int i = 2; i < arguments.size(); i++) {
                arguments[1] += " " + arguments[i];
            }
        }
        try {
            Val.insert(std::pair<std::string, std::string>(arguments[0], arguments[1]));
        }
        catch (...) {
            //Error message
        }
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
