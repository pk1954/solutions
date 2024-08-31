// Trace.cpp
//
// Win32_utilities

module Trace;

import std;
import WinBasics;

using std::cout;
using std::wofstream;
using std::ostringstream;
using std::string;
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

wstring stringToWString(const string& str) 
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), (LPWSTR)0, 0);
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