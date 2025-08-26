#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <windows.h>
#include <string>
#include <map>

// Window procedure function
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Function to display arguments
void DisplayArguments(HWND hwnd, int argc, LPWSTR* argv, 
                    std::map<std::string, std::map<std::string, std::string>>& config);

// Function to calculate window size
void CalculateWindowSize(int argc, int& width, int& height);

#endif // DISPLAY_MANAGER_H