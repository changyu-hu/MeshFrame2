#include "IO.h"

#include <iostream>
#include <fstream>
#include <deque>
#include <list>
#include <algorithm>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <direct.h>
#include <fileapi.h>
#endif

#ifdef __GNUC__
#include <unistd.h>
#include <sys/stat.h>
#endif

using namespace MF;

bool IO::folderExists(const char* folderName)
{
#ifdef _WIN32
	if (_access(folderName, 0) == -1) {
		//File not found
		return false;
	}

	DWORD attr = GetFileAttributes((LPCSTR)folderName);
	if (!(attr & FILE_ATTRIBUTE_DIRECTORY)) {
		// File is not a directory
		return false;
	}

	return true;
#endif

#ifdef __GNUC__
	if (access(folderName, 0) == -1) {
		//File not found
		return false;
	}
	return true;
#endif
}

bool IO::copyFile(std::string src, std::string dst)
{
	std::ifstream is(src, std::ios::in | std::ios::binary);
	std::ofstream os(dst, std::ios::out | std::ios::binary);

	if (is.is_open() && os.is_open()) {

		std::copy(std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>(),
			std::ostreambuf_iterator<char>(os));
		os.close();
		is.close();
		return true;
	}
	else
	{
		return false;
	}
}

bool IO::createFolder(std::string folderName)
{
	std::list<std::string> folderLevels;
	char* c_str = (char*)folderName.c_str();

	// Point to end of the string
	char* strPtr = &c_str[strlen(c_str) - 1];

	// Create a list of the folders which do not currently exist
	do {
		if (folderExists(c_str)) {
			break;
		}
		// Break off the last folder name, store in folderLevels list
		do {
			strPtr--;
		} while ((*strPtr != '\\') && (*strPtr != '/') && (strPtr >= c_str));
		folderLevels.push_front(std::string(strPtr + 1));
		strPtr[1] = 0;
	} while (strPtr >= c_str);

	#ifdef _WIN32
	if (_chdir(c_str)) {
		return true;
	}

	// Create the folders iteratively
	for (std::list<std::string>::iterator it = folderLevels.begin(); it != folderLevels.end(); it++) {
		if (CreateDirectory(it->c_str(), NULL) == 0) {
			return true;
		}
		_chdir(it->c_str());
	}
	#endif

	#ifdef __GNUC__
	if (chdir(c_str)) {
		return true;
	}
	mode_t mode = 0777;
	for (std::list<std::string>::iterator it = folderLevels.begin(); it != folderLevels.end(); it++) {
		if (mkdir(it->c_str(), mode) == 0) {
			return true;
		}
		chdir(it->c_str());
	}

	#endif

	return false;
}