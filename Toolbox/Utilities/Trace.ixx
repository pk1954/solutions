// Trace.ixx : 
//
// Toolbox\Utilities

module;

#include <fstream>
#include <iostream>

export module Trace;

using std::wofstream;
using std::wstring;

export wofstream OpenTraceFile(wstring const &);
export bool      SwitchWcoutTo(wstring const &);
