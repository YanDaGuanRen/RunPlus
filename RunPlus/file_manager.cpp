#include "file_manager.h"
#include <windows.h>
#include <vector>
#include <string>
#include <iostream>

// Get all files with specified extension in a directory
std::vector<std::wstring> GetFilesByExtension(const std::wstring& directory, const std::wstring& extension)
{
    std::vector<std::wstring> files;
    WIN32_FIND_DATA findData;
    HANDLE hFind;
    
    // Construct search path
    std::wstring searchPath = directory + L"\\*" + extension;
    
    // Find the first file
    hFind = FindFirstFile(searchPath.c_str(), &findData);
    
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            // Check if it's a file (not a directory)
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                // Add to result list
                files.push_back(findData.cFileName);
            }
        } while (FindNextFile(hFind, &findData));
        
        // Close find handle
        FindClose(hFind);
    }
    
    return files;
}

// Delete file
bool DeleteFile(const std::wstring& filePath)
{
    return ::DeleteFile(filePath.c_str()) != 0;
}

// Copy file
bool CopyFile(const std::wstring& sourcePath, const std::wstring& destPath)
{
    return ::CopyFile(sourcePath.c_str(), destPath.c_str(), FALSE) != 0;
}