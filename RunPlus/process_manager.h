#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <string>
#include <map>
#include <vector>

using std::string;

// Launch external program using WinExec
bool RunWinExec(const string& input, const  std::map<std::string, std::map<std::string, std::string>>& config);

// Launch external program using CreateProcess (recommended)
bool RunCreateProcess(const string& input, const  std::map<std::string, std::map<std::string, std::string>>& config);

// Launch external program using Cmd
bool RunCmd(const string& input, const  std::map<std::string, std::map<std::string, std::string>>& config);

#endif // PROCESS_MANAGER_H