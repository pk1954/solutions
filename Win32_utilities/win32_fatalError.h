// win32_fatalError.h
//
// NNetWindows

#pragma once

#include "windows.h"
#include <string>
#include <sstream>
#include <iostream>

using std::wstring;
using std::wostringstream;
using std::wcout;
using std::endl;

class FatalError
{
public:
	static void Happened( long const errnr, wstring const errText )
	{
		wostringstream wBuffer;
		wBuffer << L"+++ fatal error ++++++++++++"   << endl;
		wBuffer << L"+++ error number:  " << errnr   << endl;
		wBuffer << L"+++ error message; " << errText << endl;
		wcout << wBuffer.str();
		MessageBox( nullptr, wBuffer.str().c_str(), NULL, MB_OK );
		terminate();
	}

	static void Provoke( )
	{
		long long x = 5;
		int * pi = (int *)(x);
		* pi = 23;
	}

};