// AppStartProtocol.ixx
//
// Win32_utilities

module;

#include <string>
#include <iostream>
#include <Windows.h>

export module AppStartProtocol;

import Util;
import IoConstants;
import Win32_Util;

using std::wcout;
using std::wstring;
using std::endl;

export void PrintAppStartProtocol(wstring const& wstrProductName)
{
	wcout << COMMENT_START << L"Application start at " << GetCurrentDateAndTime();
	wcout << COMMENT_START << L"Version:       "       << wstrProductName     << endl;
	wcout << COMMENT_START << L"Build date:    "       << COMPILE_TIMESTAMP   << endl;
	wcout << COMMENT_START << L"Computer name: "       << ::GetComputerName() << endl;
	wcout << COMMENT_START << L"User name:     "       << ::GetUserName()     << endl;
}
