// Trace.cpp
//
// Utilities

module;

#include <io.h>
#include <fcntl.h>
#include <codecvt>
#include <windows.h>

module Trace;

import std;

using std::cout;
using std::wofstream;
using std::ostringstream;
using std::wstring;
using std::wstringstream;
using std::ios;
using std::endl;
using std::stacktrace;

wofstream OpenTraceFile(wstring const & wszTraceFileName)
{
    wofstream traceStream;
    traceStream.open(wszTraceFileName, ios::out);
    if (traceStream.bad())
    {
        cout << "Could not open trace file " << endl;
    }

    return traceStream;
}

bool SwitchWcoutTo(wstring const & wszTraceFileName)
{ 
    FILE  * fp;
    errno_t res = _wfreopen_s(&fp, wszTraceFileName.c_str(), L"w", stdout);
    _setmode(_fileno(stdout), _O_U8TEXT);  // set code page to UTF-8
    SetConsoleOutputCP(CP_UTF8);           // for printing Unicode
    return res == 0;
}


wstring stringToWString(const std::string& str) 
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
    wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

wstring StacktraceToWString() 
{
    auto st = stacktrace::current();
    wstringstream wss;
    for (const auto& frame : st) 
    {
        ostringstream oss;
        oss << frame;
        wss << stringToWString(oss.str()) << endl;
    }
    return wss.str();
}