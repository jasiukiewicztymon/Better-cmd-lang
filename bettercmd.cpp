#include <iostream>
#include <map>
#include <vector>
#include <stdexcept>
#include <string>

#include <windows.h>

#include "interpreter.h"
#include "bcin.h"

#ifndef _UNISTD_H
#define _UNISTD_H    1

#include <stdlib.h>
#include <io.h>
#include <process.h> 
#include <direct.h> 

#define srandom srand
#define random rand

#define R_OK    4       
#define W_OK    2       
#define F_OK    0   

#define access _access
#define dup2 _dup2
#define execve _execve
#define ftruncate _chsize
#define unlink _unlink
#define fileno _fileno
#define getcwd _getcwd
#define chdir _chdir
#define isatty _isatty
#define lseek _lseek

#ifdef _WIN64
#define ssize_t __int64
#else
#define ssize_t long
#endif

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#endif

#include <Lmcons.h>
#include <direct.h>
#include <limits.h>
#include <process.h>

std::string command, path = "c:\\";
std::vector<std::string> Keywords = {"cat", "cd", "clear", "cls", "color", "cmd", "date", "del", "diff", "dir", "disk",
                                     "echo", "elif", "else", "exit", "ext", "foreach", "help", "if", "loop", "ls",
                                     "mkdir", "move", "print", "powershell", "ps", "rmdir", "title"};

std::map<std::string, std::string> Val;
std::map<std::string, std::vector<std::string>> List;

int main(int argc, char* argv[]) {
    TCHAR username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    GetUserName(username, &username_len);
    std::wstring wusername(&username[0]);
    std::string usertype(wusername.begin(), wusername.end());
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTitleW(L"Better CMD");

    int userColor = 13, pathColor = 11;

    do {
        SetConsoleTextAttribute(hConsole, userColor);
        std::cout << usertype << " ";
        SetConsoleTextAttribute(hConsole, pathColor);
        std::cout << path << "\n";
        SetConsoleTextAttribute(hConsole, 15);
        bcin(command, " >", Val, List, Keywords);
        interpret(command, Val, List, path, userColor, pathColor);
    } while (command != "exit");
}
