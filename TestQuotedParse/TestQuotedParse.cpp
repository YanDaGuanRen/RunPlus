#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "../RunPlus/StringProcessor.h"
#include "../RunPlus/config_manager.h"

void testParseProgramAndArguments() {
	std::vector<std::string> testCases = {
"Tools\\Clash Verge\\clash-verge.exe aaa",
"pf\\RealVNC\\VNC Viewer\\vncviewer.exe --dddd -vvvv",
"c:\\windows\\system32\\WindowsPowerShell\\v1.0\\powershell.exe cccccccc c:\\windows\\system32\\WindowsPowerShell\\v1.0\\powershell.exe",
"pf86\\XMind\\XMind.exe \"c:\\windows\\system32\\WindowsPowerShell\\v1.0\\powershell.exe\"",
"pf\\Tencent\\QQMusic\\QQMusic.exe pf\\RealVNC\\VNC Viewer\\vncviewer.exe",
"runplus"
	};

	map<std::string, map<string,string>> Config = ReadUTF8Config("YanBinCfg.ini");

	for (int i = 0; i < testCases.size(); ++i) {
		std::string program, arguments;
		std::string processedPath = ProcessPath(testCases[i], "C:\\Users\\t1835\\OneDrive", Config);
		ParseProgramAndArguments(processedPath, program, arguments,Config);
		std::cout << "Test case " << (i + 1) << ":\n";
		std::cout << "  Input: " << testCases[i] << "\n";
		std::cout << "  Program: " << program << "\n";
		std::cout << "  Arguments: " << arguments << "\n\n";
	}
}

void testSplitStringIgnoreQuotedSpaces() {
    std::vector<std::string> testCases = {
        "arg1 arg2 arg3",
        "\"arg1 with spaces\" arg2 \"arg3 with spaces\"",
        "\"arg1 with spaces\" \"arg2 with spaces\" \"arg3 with spaces\"",
        "arg1 \"arg2 with spaces\" arg3",
        "\"single arg\"",
        "no quotes here",
        "\"\"",
        ""
    };

    for (int i = 0; i < testCases.size(); ++i) {
        std::vector<std::string> result = SplitStringIgnoreQuotedSpaces(testCases[i]);
        std::cout << "SplitStringIgnoreQuotedSpaces Test case " << (i + 1) << ":\n";
        std::cout << "  Input: " << testCases[i] << "\n";
        std::cout << "  Output: ";
        for (const auto& part : result) {
            std::cout << "[" << part << "] ";
        }
        std::cout << "\n\n";
    }
}

void testReadUTF8Config() {
    std::cout << "Testing ReadUTF8Config function:\n";
    map<string, map<string, string>> config = ReadUTF8Config("YanBinCfg.ini");
    
    // Test Settings section
    std::cout << "Settings section:\n";
    if (config.find("settings") != config.end()) {
        for (const auto& pair : config["settings"]) {
            std::cout << "  " << pair.first << " = " << pair.second << "\n";
        }
    }
    
    // Test ExecCmd section
    std::cout << "ExecCmd section:\n";
    if (config.find("execcmd") != config.end()) {
        for (const auto& pair : config["execcmd"]) {
            std::cout << "  " << pair.first << " = " << pair.second << "\n";
        }
    }
    
    // Test ExecProcess section
    std::cout << "ExecProcess section:\n";
    if (config.find("execprocess") != config.end()) {
        for (const auto& pair : config["execprocess"]) {
            std::cout << "  " << pair.first << " = " << pair.second << "\n";
        }
    }
    
    // Test Config section
    std::cout << "Config section:\n";
    if (config.find("config") != config.end()) {
        for (const auto& pair : config["config"]) {
            std::cout << "  " << pair.first << " = " << pair.second << "\n";
        }
    }
    
    std::cout << "\n";

    std::cout << "Testing GetConfigValue function:\n";

    std::cout << GetConfigValue(config, "config", "runplus") <<std::endl;

    std::cout << GetConfigValue(config, "Config", "Runplus") << std::endl;

    std::cout << GetConfigValue(config, "ConFig", "RUNplus") << std::endl;
    std::cout << "Testing GetConfigValue function End\n";
}



int main() {

    testParseProgramAndArguments();
	return 0;
}