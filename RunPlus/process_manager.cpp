#include "process_manager.h"
#include "utils.h"
#include "StringProcessor.h"
#include <windows.h>
#include <iostream>
#include <string>

using std::string;
using std::wstring;
using std::cout;
using std::endl;

// Helper function to convert std::string to std::wstring
wstring stringToWstring(const string& str) {
    if (str.empty()) return wstring();
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

// Launch external program using WinExec
bool RunWinExec(const string& input, const  std::map<std::string, std::map<std::string, std::string>>& config) {
    string programPath, arguments;
    ParseProgramAndArguments(input, programPath, arguments,config);
    
    string command = programPath;
    if (!arguments.empty()) {
        command += " ";
        command += arguments;
    }
    
    WinExec(command.c_str(), 5);
    return true;
}

// Launch external program using CreateProcess (recommended)
bool RunCreateProcess(const string& input, const  std::map<std::string, std::map<std::string, std::string>>& config) {
    string programPath, arguments;
    ParseProgramAndArguments(input, programPath, arguments,config);
    
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    
    // Initialize structures
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    
    // Construct command line arguments
    string commandLine = programPath;
    if (!arguments.empty()) {
        commandLine += " ";
        commandLine += arguments;
    }
    
    // Convert std::string to std::wstring
    wstring wCommandLine = stringToWstring(commandLine);
    
    // Create a modifiable wide string copy
    wstring command = wCommandLine;
    
    // Create process
    if (!CreateProcessW(NULL, &command[0], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        return false;
    }
    
    // Close process and thread handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return true;
}


bool RunCmd(const string& input, const  std::map<std::string, std::map<std::string, std::string>>& config) {
    string programPath, arguments;
    ParseProgramAndArguments(input, programPath, arguments,config);
    
    // Construct the full command line
    string commandLine = programPath;
    if (!arguments.empty()) {
        commandLine += " ";
        commandLine += arguments;
    }
    
    // Convert std::string to std::wstring
    wstring wCommandLine = stringToWstring(commandLine);
    
    // Create a modifiable wide string copy
    wstring command = wCommandLine;
    
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    
    // Initialize structures
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    
    // Create process
    if (!CreateProcessW(NULL, &command[0], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        cout << "Failed to create process with command line: " << commandLine << endl;
        return false;
    }
    
    cout << "Program launched successfully with CreateProcess from command line" << endl;
    
    // Close process and thread handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    
    return true;
}

