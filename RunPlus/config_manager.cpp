#include "config_manager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include "StringProcessor.h"

// Simplified std namespace types and objects
using std::string;
using std::map;
using std::ifstream;
using std::stringstream;
using std::getline;
using std::endl;
using std::cout;
using std::transform;
using std::tolower;
using std::ios;
using std::istreambuf_iterator;

// Function to read UTF-8 configuration file
map<string, map<string, string>> ReadUTF8Config(const string& filename)
{
    map<string, map<string, string>> config;
    
    // Open file in binary mode to avoid automatic conversion
    ifstream file(filename, ios::binary);
    if (!file.is_open())
    {
        return config;
    }
    
    // Read entire file content
    string content((istreambuf_iterator<char>(file)),
                        istreambuf_iterator<char>());
    file.close();
    
    // Parse configuration file
    stringstream ss(content);
    string line;
    string currentSection = "DEFAULT"; // Default section for keys without a section
    
    while (getline(ss, line))
    {
        // Skip empty lines and comment lines
        if (line.empty() || line[0] == '#')
        {
            continue;
        }
        
        // Check if it's a section line
        if (line[0] == '[' && line[line.length() - 1] == ']')
        {
            // Extract section name
            currentSection = line.substr(1, line.length() - 2);
            // Remove leading and trailing spaces
            currentSection.erase(0, currentSection.find_first_not_of(" \t"));
            currentSection.erase(currentSection.find_last_not_of(" \t") + 1);
            // Convert to lowercase for case-insensitive comparison
            currentSection = ToLower(currentSection);
            continue;
        }
        
        // Find equal sign position
        size_t pos = line.find('=');
        if (pos != string::npos)
        {
            // Extract key and value
            string key = line.substr(0, pos);
            string value = line.substr(pos + 1);
            
            // Remove leading and trailing spaces
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            
            // Convert key to lowercase for case-insensitive comparison
            key = ToLower(key);
            
            // Add to configuration map
            config[currentSection][key] = value;
        }
    }
    
    return config;
}

// Function to read specified section from UTF-8 configuration file
map<string, string> ReadUTF8ConfigSection(const string& filename, const string& targetSection)
{
    map<string, string> config;
    
    // Open file in binary mode to avoid automatic conversion
    ifstream file(filename, ios::binary);
    if (!file.is_open())
    {
        return config;
    }
    
    // Read entire file content
    string content((istreambuf_iterator<char>(file)),
                        istreambuf_iterator<char>());
    file.close();
    
    // Parse configuration file
    stringstream ss(content);
    string line;
    string currentSection;
    
    // Convert target section to lowercase for case-insensitive comparison
    string lowerTargetSection = ToLower(targetSection);
    
    while (getline(ss, line))
    {
        // Skip empty lines and comment lines
        if (line.empty() || line[0] == '#')
        {
            continue;
        }
        
        // Check if it's a section line
        if (line[0] == '[' && line[line.length() - 1] == ']')
        {
            // Extract section name
            currentSection = line.substr(1, line.length() - 2);
            // Remove leading and trailing spaces
            currentSection.erase(0, currentSection.find_first_not_of(" \t"));
            currentSection.erase(currentSection.find_last_not_of(" \t") + 1);
            // Convert to lowercase for case-insensitive comparison
            currentSection = ToLower(currentSection);
            continue;
        }
        
        // If not in target section, skip
        if (currentSection != lowerTargetSection)
        {
            continue;
        }
        
        // Find equal sign position
        size_t pos = line.find('=');
        if (pos != string::npos)
        {
            // Extract key and value
            string key = line.substr(0, pos);
            string value = line.substr(pos + 1);
            
            // Remove leading and trailing spaces
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            
            // Convert key to lowercase for case-insensitive comparison
            key = ToLower(key);
            
            // Add to configuration map
            config[key] = value;
        }
    }
    
    return config;
}

// Function to safely get value from configuration map
string GetConfigValue(const map<string, map<string, string>>& config, const string& section, const string& key)
{
    // Convert section and key to lowercase for case-insensitive comparison
    string lowerSection = ToLower(section);
    string lowerKey = ToLower(key);
    
    // Find the section
    auto sectionIt = config.find(lowerSection);
    if (sectionIt == config.end())
    {
        // Section not found, return empty string
        return "";
    }
    
    // Find the key within the section
    auto keyIt = sectionIt->second.find(lowerKey);
    if (keyIt == sectionIt->second.end())
    {
        // Key not found, return empty string
        return "";
    }
    
    // Return the value
    return keyIt->second;
}

