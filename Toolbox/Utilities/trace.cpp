// Trace.cpp
//
// Utilities

module;

#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <windows.h>

module Trace;

using std::cout;

using std::wofstream;
using std::wstring;
using std::cout;
using std::ios;
using std::endl;

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