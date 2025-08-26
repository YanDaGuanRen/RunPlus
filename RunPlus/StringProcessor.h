#ifndef STRING_PROCESSOR_H
#define STRING_PROCESSOR_H

#include <string>
#include <vector>
#include <windows.h>
#include <map>

using std::string;
using std::map;

LPWSTR* ParseCommandLine(int& argc);
std::string WrapProgramPath(const std::string& programPath);
void ParseArguments( std::string& arguments, const  std::map<std::string, std::map<std::string, std::string>>& config);
void ParseProgramAndArguments(const std::string& input, std::string& programPath, std::string& arguments, const  std::map<std::string, std::map<std::string, std::string>>& config);
std::vector<std::string> SplitStringIgnoreQuotedSpaces(const std::string& input);
std::string ProcessPath(const std::string& upath, const std::string& parentDirectory, const  std::map<std::string, std::map<std::string, std::string>>& config, bool readConfigValue = true);
// Helper function to convert a string to lowercase
string ToLower(const string& str);

#endif // STRING_PROCESSOR_H