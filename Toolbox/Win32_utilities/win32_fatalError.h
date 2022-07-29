// win32_fatalError.h
//
// NNetWindows

#pragma once

#include "Windows.h"
#include <string>
#include <codecvt>
#include <sstream>
#include <iostream>

using std::wstring;
using std::wostringstream;
using std::wcout;
using std::endl;

class FatalError
{
public:
	static void Happened(long const errnr, wstring const & errText)
	{
		wostringstream wBuffer;
		wBuffer << L"Error number:  " << errnr   << endl;
		wBuffer << L"Error message: " << errText << endl;
		wBuffer << L"Press OK to exit application" << endl;
		wcout << L"+++ Fatal error" << endl;
		wcout << L"+++ Error number:  " << errnr   << endl;
		wcout << L"+++ Error message: " << errText << endl;
		MessageBox(nullptr, wBuffer.str().c_str(), L"Fatal error", MB_OK|MB_SYSTEMMODAL);
		wcout << L"+++ forced application exit" << endl;
		ExitProcess(3);
	}

	static void Provoke()
	{
		long long x = 5;
		int * pi = (int *)(x); //-V566
		* pi = 23;
	}

};