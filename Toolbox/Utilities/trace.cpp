// Trace.cpp
//
// Utilities

module;

#include <string>
#include <fstream>
#include <iostream>

module Trace;

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
    return res == 0;
}
