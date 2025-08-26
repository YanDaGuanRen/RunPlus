#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>

// Logger class
class Logger {
private:
    std::ofstream logFile;
    bool fileOpened;

public:
    Logger();
    Logger(const std::string& logFileName);
    ~Logger();
    void Log(const std::string& message);
};

#endif // LOGGER_H