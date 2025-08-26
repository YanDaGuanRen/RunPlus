#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <string>
#include <map>

// Simplified std namespace types and objects
using std::string;
using std::map;

// Function to read UTF-8 configuration file
map<string, map<string, string>> ReadUTF8Config(const string& filename);

// Function to read a specific section from a UTF-8 configuration file
map<string, string> ReadUTF8ConfigSection(const string& filename, const string& section);

// Function to safely get a value from the configuration map
string GetConfigValue(const map<string, map<string, string>>& config, const string& section, const string& key);


#endif // CONFIG_MANAGER_H