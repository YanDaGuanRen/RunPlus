#include "utils.h"
#include "StringProcessor.h"
#include <windows.h>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

using std::string;

string TrimStartEnd(const string& trimmedInput) {
    
    //Implement the operation of removing blank characters at both ends of a string
    size_t start = trimmedInput.find_first_not_of(' ');
    size_t end = trimmedInput.find_last_not_of(' ');
    if (start != string::npos && end != string::npos) {
       return trimmedInput.substr(start, end - start + 1);
    }
    else if (start != string::npos) {
        return trimmedInput.substr(start);
    }
}

ProgramInfo ExtractProgramInfo() {
    ProgramInfo info;
    
    wchar_t szPath[MAX_PATH];
    GetModuleFileNameW(NULL, szPath, MAX_PATH);
    
    int len = WideCharToMultiByte(CP_UTF8, 0, szPath, -1, NULL, 0, NULL, NULL);
    info.path.resize(len - 1);
    WideCharToMultiByte(CP_UTF8, 0, szPath, -1, &info.path[0], len, NULL, NULL);
    
    size_t lastSlash = info.path.find_last_of("\\");
    info.name = (lastSlash != string::npos) ? info.path.substr(lastSlash + 1) : info.path;
    info.directory = (lastSlash != string::npos) ? info.path.substr(0, lastSlash) : "";
    
    size_t lastDot = info.name.find_last_of(".");
    if (lastDot != string::npos) {
        info.name = info.name.substr(0, lastDot);
    }
    
    return info;
}

// Removed parameterized constructor

// Removed Enable method