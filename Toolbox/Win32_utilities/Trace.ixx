// Trace.ixx : 
//
// Win32_utilities

export module Trace;

import std;

using std::wofstream;
using std::wstring;

export wofstream OpenTraceFile(wstring const &);
export wstring   StacktraceToWString();
