#include "StringProcessor.h"
#include "config_manager.h"
#include <windows.h>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <shlobj.h>
#include <algorithm>
#include "utils.h"
#include <regex>

using std::string;
using std::cout;
using std::endl;
using std::transform;
using std::tolower;

LPWSTR* ParseCommandLine(int& argc) {
	return CommandLineToArgvW(GetCommandLineW(), &argc);
}

std::string WrapProgramPath(const std::string& programPath) {
	if (programPath.find(' ') != std::string::npos &&
		(programPath.front() != '"' || programPath.back() != '"')) {
		return "\"" + programPath + "\"";
	}
	return programPath;
}

void ParseArguments(std::string& arguments, const std::map<std::string, std::map<std::string, std::string>>& config)
{
	if (arguments.empty()) {
		return;
	}
	string trimmedInput = TrimStartEnd(arguments);

	// 正则表达式：匹配指定开头且以.exe结尾的字符串
	std::regex pattern(R"((pf86\\|pf\\|\.\./|\.\.\\|AppData).*?\.exe)");

	std::string result; // 最终结果字符串
	size_t lastPos = 0; // 记录上一个匹配项的结束位置

	// 遍历所有匹配项
	std::sregex_iterator it(trimmedInput.begin(), trimmedInput.end(), pattern);
	std::sregex_iterator end;

	while (it != end) {
		// 获取当前匹配项的位置信息
		const std::smatch& match = *it;
		size_t startPos = match.position(); // 匹配项在原字符串中的起始位置
		size_t length = match.length();     // 匹配项的长度

		// 拼接：上一个匹配结束位置 到 当前匹配开始位置之间的非匹配内容
		result += trimmedInput.substr(lastPos, startPos - lastPos);

		// 调用自定义方法处理匹配项，并拼接到结果中
		std::string processed = ProcessPath(match.str(), GetConfigValue(config, "Settings", "RunParentDirectory"), config,false);
		result += processed;

		// 更新上一个匹配结束位置
		lastPos = startPos + length;

		++it; // 移动到下一个匹配项
	}

	// 拼接最后一个匹配项之后的剩余内容
	result += trimmedInput.substr(lastPos);
	std::regex pattern1(R"(.[:].*?\.exe)");
	result = std::regex_replace(result, pattern1, R"("$&")");

	std::regex pattern3(R"("")");
	// 替换为单个双引号，R"(")"表示原始字符串中的"
	arguments = std::regex_replace(result, pattern3, R"(")");
	


	//std::vector<std::string> result = SplitStringIgnoreQuotedSpaces(trimmedInput);
 //   if (result.empty() || result.size()  <1) {
	//	return;
	//}
	//arguments = ProcessPath(result[0],GetConfigValue(config,"Settings","RunParentDirectory"),config,false);


	//for (size_t i = 1; i < result.size(); i++)
	//{
	//	string temp = ProcessPath(result[1], GetConfigValue(config, "Settings", "RunParentDirectory"), config, false);
	//	if (temp.length() >= 4 &&
	//		temp.substr(temp.length() - 4) == ".exe") {
	//		temp = "\"" + temp + "\"";
	//	}
	//	arguments += " "+temp;
	//}

}


void ParseProgramAndArguments(const string& input, string& programPath, string& arguments, const map<string, map<string, string>>& config) {
	string trimmedInput = TrimStartEnd(input);

	if (!trimmedInput.empty() && trimmedInput.front() == '"') {
		size_t endQuotePos = trimmedInput.find('"', 1);
		if (endQuotePos != string::npos) {
			programPath = trimmedInput.substr(0, endQuotePos + 1);

			size_t argsStart = endQuotePos + 1;
			while (argsStart < trimmedInput.length() && trimmedInput[argsStart] == ' ') {
				argsStart++;
			}

			if (argsStart < trimmedInput.length()) {
				arguments = trimmedInput.substr(argsStart);
			}
			else {
				arguments = "";
			}

		}
		else {
			programPath = trimmedInput;
			arguments = "";
		}
	}
	else {
		size_t exeSpacePos = trimmedInput.find(".exe ");
		size_t exeSpacePos2 = trimmedInput.find(".exe");
		size_t exeSpacePos3 = trimmedInput.find(' ');
		if (exeSpacePos != string::npos) {
			programPath = trimmedInput.substr(0, exeSpacePos + 4);
			arguments = trimmedInput.substr(exeSpacePos + 5);
		}
		else if (exeSpacePos2 != string::npos)
		{
			programPath = trimmedInput.substr(0, exeSpacePos2 + 4);
			arguments = trimmedInput.substr(exeSpacePos2 + 4);
		}
		else if (exeSpacePos3 != string::npos)
		{
			programPath = trimmedInput.substr(0, exeSpacePos3);
			arguments = trimmedInput.substr(exeSpacePos3 + 1);
		}
		else {
			programPath = trimmedInput;
			arguments = "";
		}
	}

	

	if (programPath.length() >= 4 &&
		programPath.substr(programPath.length() - 4) == ".exe") {
		programPath = "\"" + programPath + "\"";
	}

	ParseArguments(arguments,config);

}



std::string ProcessPath(const std::string& upath, const std::string& parentDirectory,const  map<string, map<string, string>>& config,bool readConfigValue) {

	// If the path is empty or null, return directly
	if (upath.empty()) {
		return upath;
	}

	if (readConfigValue && !config.empty()) {
		string lowerPath = ToLower(upath);
		if (!(lowerPath = GetConfigValue(config, "config",upath)).empty()) {
			return ProcessPath(lowerPath, parentDirectory, config);
		}
	}

	// If the path is an absolute path (contains drive letter)
	if (upath.length() > 3 && upath[1] == ':' && upath[2] == '\\') {
		return upath;
	}
	if (upath.length() > 4 && upath[0] == '"' && upath[2] == ':' && upath[3] == '\\') {
		return upath;
	}

	// If the path is an HTTP URL
	if (upath.length() >= 4 && upath.substr(0, 4) == "http") {
		return upath;
	}

	// If the path starts with a backslash, remove the first character
	std::string processedPath = upath;
	std::string processedParentDirectory = parentDirectory;
	bool addProgramPath = true;

	while (processedPath.length() > 0 && processedPath[0] == '\\') {
		processedPath = processedPath.substr(1);
	}

	// If the path starts with "AppData"
	if (processedPath.length() >= 7 && processedPath.substr(0, 7) == "AppData") {
		// Remove the "AppData" par
		processedPath = processedPath.substr(7);

		// Get the parent directory of the application data directory
		char appDataPath[MAX_PATH];
		if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, appDataPath))) {
			processedParentDirectory = std::string(appDataPath);
		}

	}
	// If the path starts with "..\\" (relative path)
	else if (processedPath.length() >= 3 && processedPath.substr(0, 3) == "..\\") {
		// Loop to handle all "..\\"
		while (processedPath.length() >= 3 && processedPath.substr(0, 3) == "..\\") {
			// Get the parent directory of the parent directory
			size_t lastSlash = processedParentDirectory.find_last_of('\\');
			if (lastSlash != std::string::npos) {
				processedParentDirectory = processedParentDirectory.substr(0, lastSlash);
			}

			// Remove "..\\" from the path
			processedPath = processedPath.substr(3);
		}
	}
	// If the path starts with "../" (relative path)
	else if (processedPath.length() >= 3 && processedPath.substr(0, 3) == "../") {
		// Loop to handle all "../"
		while (processedPath.length() >= 3 && processedPath.substr(0, 3) == "../") {
			// Get the parent directory of the parent directory
			size_t lastSlash = processedParentDirectory.find_last_of('/');
			if (lastSlash == std::string::npos) {
				lastSlash = processedParentDirectory.find_last_of('\\');
			}

			if (lastSlash != std::string::npos) {
				processedParentDirectory = processedParentDirectory.substr(0, lastSlash);
			}

			// Remove "../" from the path
			processedPath = processedPath.substr(3);

		}
	}
	// If the path starts with "pf\\" (Program Files abbreviation)
	else if (processedPath.length() >= 3 && processedPath.substr(0, 3) == "pf\\") {
		addProgramPath = true;
		// Remove the "pf\\" part
		processedPath = processedPath.substr(3);

		// Iterate through drive letters C to G
		for (char c = 'C'; c <= 'G'; c++) {
			// Construct the Program Files directory path
			std::string programFilesPath = std::string(1, c) + ":\\Program Files\\";

			// Check if the file exists at this path
			std::string fullPath = programFilesPath + processedPath;
			DWORD attributes = GetFileAttributesA(fullPath.c_str());
			if (attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY)) {
				// Construct the full path and break the loop
				processedPath = fullPath;
				break;
			}
		}
	}
	// If the path starts with "pf86\\" (Program Files (x86) abbreviation)
	else if (processedPath.length() >= 5 && processedPath.substr(0, 5) == "pf86\\") {
		
		// Remove the "pf86\\" part
		processedPath = processedPath.substr(5);

		// Iterate through drive letters C to G
		for (char c = 'C'; c <= 'G'; c++) {
			// Construct the Program Files (x86) directory path
			std::string programFilesPath = std::string(1, c) + ":\\Program Files (x86)\\";

			// Check if the file exists at this path
			std::string fullPath = programFilesPath + processedPath;
			DWORD attributes = GetFileAttributesA(fullPath.c_str());
			if (attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY)) {
				// Construct the full path and break the loop
				processedPath = fullPath;
				break;
			}
		}
	}
	else
	{
		if (processedPath.length() > 0 && processedPath.find_first_of(".exe") == std::string::npos)
		{
			addProgramPath = false;
		}
		
	}

	// If the path is not an absolute path (does not contain drive letter)
	if (processedPath.length() > 3 && processedPath[1] != ':') {
		// Remove leading backslashes
		while (!processedPath.empty() && processedPath[0] == '\\') {
			processedPath = processedPath.substr(1);
		}

		if (addProgramPath)
		{
			// Combine with parent directory
			processedPath = processedParentDirectory + "\\" + processedPath;
		}
	}

	return processedPath;
}

std::vector<std::string> SplitStringIgnoreQuotedSpaces(const std::string& input) {
    std::vector<std::string> result;
    std::string current;
    bool inQuotes = false;
    
    for (size_t i = 0; i < input.length(); ++i) {
        char c = input[i];
        
        if (c == '"') {
            inQuotes = !inQuotes;
            current += c;
        } else if (c == ' ' && !inQuotes) {
            if (!current.empty()) {
                result.push_back(current);
                current.clear();
            }
        } else {
            current += c;
        }
    }
    
    if (!current.empty()) {
        result.push_back(current);
    }
    
    return result;
}

// Helper function to convert string to lowercase
string ToLower(const string& str)
{
	string lowerStr = str;
	transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
		[](unsigned char c) { return tolower(c); });
	return lowerStr;
}