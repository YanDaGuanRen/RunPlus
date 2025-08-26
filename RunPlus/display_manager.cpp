#include "display_manager.h"
#include <windows.h>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <iostream>
#include "config_manager.h"
#include "StringProcessor.h"

// Global variable to store text to be displayed
std::wstring g_displayText;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		// Add debug information
		std::cerr << "Window created" << std::endl;
		break;
	case WM_DESTROY:
		// Add debug information
		std::cerr << "Window destroyed" << std::endl;
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		// Set font
		HFONT hFont = CreateFont(
			16, 0, 0, 0,
			FW_NORMAL,
			FALSE, FALSE, FALSE,
			DEFAULT_CHARSET,
			OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_SWISS,
			L"Microsoft YaHei"
		);
		HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

		// Draw text
		RECT rect;
		GetClientRect(hwnd, &rect);
		// Ensure the rectangle bottom is large enough to display all text
		rect.bottom = rect.top + 1000;  // Increase height to ensure all text can be displayed
		DrawText(hdc, g_displayText.c_str(), -1, &rect, DT_LEFT | DT_TOP | DT_WORDBREAK);

		// Restore old font
		SelectObject(hdc, hOldFont);
		DeleteObject(hFont);

		EndPaint(hwnd, &ps);
	}
	break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void CalculateWindowSize(int argc, int& width, int& height)
{
	// Basic window size
	width = 800;  // Increase width to accommodate more content
	height = 300; // Increase basic height

	// Adjust window size based on number of arguments
	if (argc > 0)
	{
		height += argc * 40;  // Increase height by 40 pixels for each argument
	}

	// Ensure window is not too large
	if (height > 1000)
	{
		height = 1000;
	}

	// Ensure window is not too small
	if (height < 300)
	{
		height = 300;
	}
}

void DisplayArguments(HWND hwnd, int argc, LPWSTR* argv, std::map<std::string, std::map<std::string, std::string>>& config)
{
	// Removed debug MessageBox

	// Build text to display
	std::wstringstream ss;

	if (argc == 0)
	{
		ss << L"No arguments provided.\n\nPlease provide arguments when starting the program, for example:\nRunPlus.exe arg1 arg2 arg3";
	}
	else
	{
		ss << L"Number of arguments: " << argc << L"\n\n";

		for (int i = 0; i < argc; i++)
		{
			ss << L"Argument " << (i + 1) << L": " << argv[i] << L"\n";
		}
	}

	// Add debug information to output built text content
	std::wstring debugText = ss.str();
	// Convert wide string to multi-byte string for debug output
	int debugLen = WideCharToMultiByte(CP_ACP, 0, debugText.c_str(), -1, NULL, 0, NULL, NULL);
	std::string debugTextA(debugLen, 0);
	WideCharToMultiByte(CP_ACP, 0, debugText.c_str(), -1, &debugTextA[0], debugLen, NULL, NULL);
	std::cerr << "Debug: Built text content:\n" << debugTextA << std::endl;
	// Test Settings section
	ss << L"\nSettings section:\n";
	if (config.find("settings") != config.end()) {
		for (const auto& pair : config["settings"]) {
			ss << L"  " << pair.first.c_str() << L" = " << pair.second.c_str() << L"\n";
		}
	}

	// Test ExecCmd section
	ss << L"ExecCmd section:\n";
	if (config.find("execcmd") != config.end()) {
		for (const auto& pair : config["execcmd"]) {
			ss << L"  " << pair.first.c_str() << L" = " << pair.second.c_str() << L"\n";
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
	ss << L"Config section:\n";
	if (config.find("config") != config.end()) {
		for (const auto& pair : config["config"]) {
			std::cout << "  " << pair.first << " = " << pair.second << "\n";
		}
	}

	ss << "\n";


	// Get device context to calculate text size
	HDC hdc = GetDC(hwnd);
	HFONT hFont = CreateFont(
		16, 0, 0, 0,
		FW_NORMAL,
		FALSE, FALSE, FALSE,
		DEFAULT_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		L"Microsoft YaHei"
	);
	HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

	// Calculate text size
	RECT rect = { 0, 0, 1000, 0 };  // Increase width to accommodate more content
	std::wstring wText = ss.str();
	// When calculating text size, do not use DT_WORDBREAK to correctly calculate required space
	DrawText(hdc, wText.c_str(), -1, &rect, DT_CALCRECT | DT_LEFT | DT_TOP);

	// Restore old font
	SelectObject(hdc, hOldFont);
	ReleaseDC(hwnd, hdc);

	// Adjust rectangle size to include margins
	rect.right += 40;  // Increase margin
	rect.bottom += 40; // Increase margin

	// Ensure minimum size
	if (rect.right < 300) rect.right = 300;   // Increase minimum width
	if (rect.bottom < 200) rect.bottom = 200; // Increase minimum height

	// Save text to global variable
	g_displayText = wText;

	// Adjust window size to fit content
	RECT windowRect;
	GetWindowRect(hwnd, &windowRect);
	int windowWidth = rect.right + 20;
	int windowHeight = rect.bottom + 40;  // Extra space for title bar etc

	// Ensure window is not too large
	if (windowWidth > 1000) windowWidth = 1000;
	if (windowHeight > 800) windowHeight = 800;

	SetWindowPos(hwnd, NULL, 0, 0, windowWidth, windowHeight, SWP_NOMOVE | SWP_NOZORDER);

	// Trigger repaint
	InvalidateRect(hwnd, NULL, TRUE);
}