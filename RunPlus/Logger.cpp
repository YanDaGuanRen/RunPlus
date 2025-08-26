#include "Logger.h"
#include <windows.h>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

using std::string;

Logger::Logger() : Logger("Run.log") {}

Logger::Logger(const std::string& logFileName) : fileOpened(false) {
    // Get the full path of the current program
    wchar_t szPath[MAX_PATH];
    GetModuleFileNameW(NULL, szPath, MAX_PATH);
    
    // Convert wide string to multi-byte string
    int len = WideCharToMultiByte(CP_UTF8, 0, szPath, -1, NULL, 0, NULL, NULL);
    std::string path(len - 1, 0);  // -1 to exclude null terminator
    WideCharToMultiByte(CP_UTF8, 0, szPath, -1, &path[0], len, NULL, NULL);
    
    // Extract directory
    size_t lastSlash = path.find_last_of("\\");
    std::string directory = (lastSlash != string::npos) ? path.substr(0, lastSlash) : "";
    
    // Construct log file path
    std::string logPath = directory + "\\" + logFileName;
    
    // Open log file
    logFile.open(logPath, std::ios::out | std::ios::app);
    if (logFile.is_open()) {
        fileOpened = true;
        
        // Write log start marker
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        
        std::ostringstream oss;
        // Use localtime_s instead of localtime to avoid security warnings
        std::tm localTime;
        localtime_s(&localTime, &time_t);
        oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
        oss << "." << std::setfill('0') << std::setw(3) << ms.count();
        
        Log("===== Session Started at " + oss.str() + " =====");
    } else {
        // Show error message
        MessageBoxW(NULL, (L"Failed to open log file: " + std::wstring(logPath.begin(), logPath.end())).c_str(), L"Error", MB_OK | MB_ICONERROR);
    }
    // If unable to open log file, fileOpened remains false
}

Logger::~Logger() {
    if (fileOpened) {
        // Write log end marker
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        
        std::ostringstream oss;
        // Use localtime_s instead of localtime to avoid security warnings
        std::tm localTime;
        localtime_s(&localTime, &time_t);
        oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
        oss << "." << std::setfill('0') << std::setw(3) << ms.count();
        
        Log("===== RunPlus Session Ended at " + oss.str() + " =====");
        
        logFile.close();
    }
}

void Logger::Log(const std::string& message) {
    if (fileOpened) {
        // Get current time
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        
        std::ostringstream oss;
        // Use localtime_s instead of localtime to avoid security warnings
        std::tm localTime;
        localtime_s(&localTime, &time_t);
        oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
        oss << "." << std::setfill('0') << std::setw(3) << ms.count();
        
        // Write to log
        logFile << "[" << oss.str() << "] " << message << std::endl;
        logFile.flush();
    }
}