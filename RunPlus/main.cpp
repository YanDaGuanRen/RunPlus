#include <windows.h>
#include <string>
#include <map>
#include <iostream>
#include <locale>
#include <codecvt>
#include <sstream>
#include "config_manager.h"
#include "display_manager.h"
#include "file_manager.h"
#include "utils.h"
#include "process_manager.h"
#include "logger.h"
#include "StringProcessor.h"

// Simplified std namespace types and objects
using std::string;
using std::map;
using std::cout;
using std::endl;

// Global logger object pointer
Logger* g_logger = nullptr;


// Debug output function
void DebugOutput(const std::string& message) {
	// Log to file
	if (g_logger) {
		g_logger->Log(message);
	}
}


// Program exit function
void ExitApplication(int exitCode = 0) {
	// Release Logger instance
	if (g_logger) {
		delete g_logger;
		g_logger = nullptr;
	}

	// Add other resources that need to be released here

	// Exit program
	exit(exitCode);
}



// Function to test reading configuration files
void TestSectionReading() {
	// Test code can be added here to verify configuration reading functionality
	// For example:
	// map<wstring, wstring> settingsConfig = ReadUTF8ConfigSection("YanBinCfg.ini", L"Settings");
	// wcout << L"Value from config: " << (settingsConfig.find(L"KeyName") != settingsConfig.end() ? settingsConfig[L"KeyName"] : L"") << endl;
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	// Check command line arguments to see if window should be displayed
	// If a console is attached, we still execute full logic but don't show window
	bool showWindow = true;

	// Parse command line arguments
	int wargc;
	LPWSTR* wargv = ParseCommandLine(wargc);

	// Check for --yanbin parameter
	bool debugMode = false;
	for (int i = 1; i < wargc; i++) {
		if (wcscmp(wargv[i], L"--debug") == 0) {
			debugMode = true;
			break;
		}
	}

	debugMode = false;

	if (debugMode) {
		g_logger = new Logger("CustomLog.log");
	}

	DebugOutput("Starting RunPlus");

	// Read configuration sections
	DebugOutput("Reading config sections");
	map<string, map<string, string>> config = ReadUTF8Config("YanBinCfg.ini");
	DebugOutput("Get program information:");
	ProgramInfo programInfo = ExtractProgramInfo();
	string programPath = programInfo.path;
	string programName = programInfo.name;
	string programDirectory = programInfo.directory;
	DebugOutput("Program path: " + programPath);
	DebugOutput("Program name: " + programName);
	DebugOutput("Program directory: " + programDirectory);


	string configProgramName;
	DebugOutput("Get configuration information for program '" + programName + "'");

	// Find configuration in order of priority
	if (!(configProgramName = GetConfigValue(config, "ExecProcess", programName)).empty()) {

	}
	else if (!(configProgramName = GetConfigValue(config, "ExecCmd", programName)).empty()) {
		DebugOutput("Found configuration in ExecCmd");
	}
	else if (!(configProgramName = GetConfigValue(config, "Config", programName)).empty()) {
		DebugOutput("Found configuration in Config");
	}
	else {
		DebugOutput("Error: Configuration for program '" + programName + "' not found in any configuration section");
		ExitApplication(1);
	}
	// Output debug information (using conversion function)
	DebugOutput("Value of parameter " + programName + ": " + configProgramName);

	//// Test launching program with parameters using CreateProcess
	//DebugOutput("Launching program using RunCmd");
	//RunCmd("C:\\Program Files (x86)\\Microsoft\\Edge\\Application\\msedge.exe https://www.example1.com");
	//DebugOutput("Launching program using RunCreateProcess");
	//RunCreateProcess("C:\\Program Files (x86)\\Microsoft\\Edge\\Application\\msedge.exe https://www.example2.com");



	// Display arguments and configuration information
	DebugOutput("Displaying arguments");
	// Note: In console mode, we don't have a window handle, so pass NULL
	DisplayArguments(NULL, wargc - 1, wargv + 1, ConfigX);

	if (!showWindow) {
		DebugOutput("Running in console mode without window");
		// Execute specific logic in console mode
		// For example, output program information etc.
		DebugOutput("Program path: " + programPath);
		DebugOutput("Program name: " + programName);
		DebugOutput("Program directory: " + programDirectory);

		// In console mode, no window creation and message loop is needed
		// But for testing, we pause for a few seconds to observe output
		DebugOutput("Pausing for 5 seconds to observe output...");
		Sleep(5000);  // Pause for 5 seconds
		DebugOutput("Exiting console mode");
		ExitApplication(0);
	}

	DebugOutput("Creating window...");

	// Register window class
	DebugOutput("Registering window class");
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = L"MainWindowClass";

	if (!RegisterClass(&wc)) {
		DebugOutput("Failed to register window class");
		ExitApplication(0);
	}

	// Create main window
	DebugOutput("Creating window");
	HWND hwnd = CreateWindowEx(
		0,
		L"MainWindowClass",
		L"RunPlus",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
		NULL, NULL, GetModuleHandle(NULL), NULL
	);

	if (hwnd == NULL) {
		DebugOutput("Failed to create window");
		ExitApplication(0);
	}

	// Show window
	DebugOutput("Showing window");
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	// Display arguments and configuration information
	DebugOutput("Displaying arguments");
	DisplayArguments(hwnd, wargc - 1, wargv + 1, ConfigX);

	// Message loop
	DebugOutput("Entering message loop");
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	DebugOutput("Exiting RunPlus");
	ExitApplication((int)msg.wParam);
}