// trace.h : 
//

#pragma once

#include <fstream>
#include <iostream>

using std::wofstream;
using std::wstring;

wofstream OpenTraceFile(wstring const &);

bool SwitchWcoutTo(wstring const &);
