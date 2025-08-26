#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <windows.h>

// Simplified std namespace types and objects
using std::string;

struct ProgramInfo {
    string path;
    string name;
    string directory;
};

string TrimStartEnd(const string& trimmedInput);

ProgramInfo ExtractProgramInfo();

#endif // UTILS_H