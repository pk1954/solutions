// Trace.ixx : 
//
// Toolbox\Utilities

export module Trace;

import std;

using std::wofstream;
using std::wstring;

export wofstream OpenTraceFile(wstring const &);
export bool      SwitchWcoutTo(wstring const &);
