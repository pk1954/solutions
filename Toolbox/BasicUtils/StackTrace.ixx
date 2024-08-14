// Debug.ixx 
//
// Toolbox\BasicUtils

export module StackTrace;

import std;
import Util;

using std::wstring;
using std::wstringstream;
using std::ostringstream;
using std::endl;
using std::stacktrace;

export wstring StacktraceToWString() 
{
    auto st = stacktrace::current();
    wstringstream wss;
    for (const auto& frame : st) 
    {
        ostringstream oss;
        oss << frame;
        wss << ConvertToWideString(oss.str()) << endl;
    }
    return wss.str();
}
