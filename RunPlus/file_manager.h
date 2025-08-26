#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <vector>
#include <windows.h>

// Get all files with specified extension in a directory
std::vector<std::wstring> GetFilesByExtension(const std::wstring& directory, const std::wstring& extension);

// Delete file
bool DeleteFile(const std::wstring& filePath);

// Copy file
bool CopyFile(const std::wstring& sourcePath, const std::wstring& destPath);

#endif // FILE_MANAGER_H