// MathLibrary.cpp : Defines the exported functions for the DLL.
#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <windows.h>
#include <vector>
#include <filesystem>
#include "FastFileFinder.h"

using namespace std;
namespace fs = std::filesystem;

// DLL internal state variables:
static std::vector<wstring> files;
static std::vector<wstring> folders;
extern "C" uint64_t files_number = 0;
extern "C" uint64_t folders_number = 0;
bool issavingresults = false;
bool hastoincludefolders = false;
bool currentlysearching = false;
bool stop = false;

std::string wstring_to_string(const std::wstring& wstr) {
    int len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string str(len, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], len, nullptr, nullptr);
    str.pop_back();
    return str;
}

char* string_to_char_array(string str)
{
    size_t length = str.length();
    char* char_array = new char[length + 1];
    str.copy(char_array, length);
    char_array[length] = '\0';
    return char_array;
}

std::wstring string_to_wstring(const std::string& str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

void listFiles(wstring path)
{
    if (!stop)
    {
        wstring itemPath;

        HANDLE hFind;
        WIN32_FIND_DATA data;

        hFind = FindFirstFile((path + L"\\*").c_str(), &data);
        if (hFind != INVALID_HANDLE_VALUE) {
            do {
                if (wcscmp(data.cFileName, L".") == 0 || wcscmp(data.cFileName, L"..") == 0) {
                    continue;
                }
                itemPath = path + L"\\" + data.cFileName;
                if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    if (hastoincludefolders)
                    {
                        folders_number++;
                        folders.push_back(itemPath);
                    }
                }
                else
                {
                    files_number++;
                    files.push_back(itemPath);
                }

                if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    listFiles(itemPath);
                }
            } while (FindNextFile(hFind, &data));

            FindClose(hFind);
        }
    }
}

char* getItems(char* path, bool include_folders)
{
    stop = false;
    files_number = 0;
    folders_number = 0;
    hastoincludefolders = include_folders;
    files.clear();
    folders.clear();
    currentlysearching = true;
    listFiles(string_to_wstring(path));
    currentlysearching = false;

    issavingresults = true;
    string out = "";
    for (const wstring& s : files)
        out += "?" + wstring_to_string(s);

    out += "*";
    for (const wstring& s : folders)
        out += "?" + wstring_to_string(s);
    issavingresults = false;

    // la struttura della stringa di output è la seguente
    // C:\Users\Kikkiu\Desktop\file1.txt?C:\Users\Kikkiu\Desktop\file2.txt:C:\Users\Kikkiu\Desktop\folder1.txt?C:\Users\Kikkiu\Desktop\folder2.txt
    // "?" separa ogni file, mentre "*" separa file da cartelle. Facendo .Split("*"), [0] è la stringa contenente i file, [1]
    // è quella contenente le cartelle

    return string_to_char_array(out);
}

uint64_t getFilesNumber()
{
    return files_number;
}

uint64_t getFoldersNumber()
{
    return folders_number;
}

bool isSearching()
{
    return currentlysearching;
}

bool isSavingResults()
{
    return issavingresults;
}

void stopSearch()
{
    stop = true;
}
