// trace.cpp
//
// Utilities

#include <assert.h>
#include <string>
#include <fstream>
#include <iostream>
#include "trace.h"

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
