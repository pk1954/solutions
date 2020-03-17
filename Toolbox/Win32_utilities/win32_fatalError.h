// win32_fatalError.h
//
// NNetWindows

#pragma once

#include "windows.h"
#include <string>
#include <codecvt>
#include <sstream>
#include <iostream>

using std::string;
using std::ostringstream;
using std::cout;
using std::endl;

class FatalError
{
public:
	static void Happened( long const errnr, string const errText )
	{
		ostringstream wBuffer;
		wBuffer << "+++ fatal error"   << endl;
		wBuffer << "+++ error number:  " << errnr   << endl;
		wBuffer << "+++ error message: " << errText << endl;
		cout << wBuffer.str();
		MessageBoxA( nullptr, wBuffer.str().c_str(), NULL, MB_OK );
		terminate();
	}

	static void Provoke( )
	{
		long long x = 5;
		int * pi = (int *)(x);
		* pi = 23;
	}

};