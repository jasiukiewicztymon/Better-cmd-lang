#include <iostream>
#include <map>
#include <vector>
#include <stdexcept>
#include <string>

#include "interpreter.h"

std::string command;
int exited;

std::map<std::string, std::string> Val;
std::map<std::string, std::vector<std::string>> List;

int main(int argc, char* argv[]) {
    do {
        std::getline(std::cin, command);
        interpret(command, Val, List);
    } while (command != "exit");
}
