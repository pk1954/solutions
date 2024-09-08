// StdOutput.ixx
//
// Toolbox\WinDefs

module;

#include <io.h>
#include <fcntl.h>
#include <codecvt>
#include <Windows.h>

export module WinBasics:StdOutput;

import std;

using std::wstring;

export bool SwitchWcoutTo(wstring const & wszTraceFileName)
{ 
    FILE  * fp;
    errno_t res = _wfreopen_s(&fp, wszTraceFileName.c_str(), L"w", stdout);
    _setmode(_fileno(stdout), _O_U8TEXT);  // set code page to UTF-8
    SetConsoleOutputCP(CP_UTF8);           // for printing Unicode
    return res == 0;
}
