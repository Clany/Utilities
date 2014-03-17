#ifndef __CLANY_FILE_HANDLE_HPP__
#define __CLANY_FILE_HANDLE_HPP__

#include <fstream>
#include <sstream>
#include <string>
#include <iterator>
#include <limits>

#if defined(WIN32) || defined(_WIN32)
   #include <windows.h>
   #include <tchar.h>
#else
   #include <dirent.h>
#endif

#include "clany/clany_macros.h"

#if defined (min) || defined (max)
#undef max
#undef min
#endif

_CLANY_BEGIN
using ifsbuf_iter = istreambuf_iterator<char>;

inline string fileToString(const string& file_name)
{
    ifstream ifs {file_name};
    return string {ifsbuf_iter{ifs}, ifsbuf_iter{}};
}


inline string readFile(const string& file_name)
{
    return fileToString(file_name);
}


inline ifstream& gotoLine(ifstream& file, int num)
{
    file.seekg(ios::beg);
    for (int i = 0; i < num; ++i) {
        file.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return file;
}


inline string getLineStr(ifstream& file, int num)
{
    gotoLine(file, num);

    string curr_line;
    getline(file, curr_line);

    return curr_line;
}


class Directory
{
public:
    static std::vector<std::string> GetListFiles  ( const std::string& path, const std::string & exten = "*", bool addPath = true );
    static std::vector<std::string> GetListFilesR ( const std::string& path, const std::string & exten = "*", bool addPath = true );
    static std::vector<std::string> GetListFolders( const std::string& path, const std::string & exten = "*", bool addPath = true );
};
_CLANY_END

#endif