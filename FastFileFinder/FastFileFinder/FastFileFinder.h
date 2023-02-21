#pragma once

#ifdef FASTFILEFINDER_EXPORTS
#define FASTFILEFINDER_API __declspec(dllexport)
#else
#define FASTFILEFINDER_API __declspec(dllimport)
#endif

#include <iostream>
#include <windows.h>
#include <tuple>
#include <vector>

using namespace std;

// Questa funzione crea una lista di tutti i file e cartelle a partire dal percorso specificato (path)
extern "C" FASTFILEFINDER_API void stopSearch();
extern "C" FASTFILEFINDER_API char* getItems(char* path, bool include_folders = true);
extern "C" FASTFILEFINDER_API uint64_t getFilesNumber();
extern "C" FASTFILEFINDER_API uint64_t getFoldersNumber();
extern "C" FASTFILEFINDER_API bool isSearching();
extern "C" FASTFILEFINDER_API bool isSavingResults();
