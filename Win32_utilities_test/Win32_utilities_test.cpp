// Win32_utilities_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "script.h"
#include "win32_winManager.h"

using namespace std;

void DoTest( )
{
//	WinManager winManager;


	wcout << endl << L"*** Win32_utilities_test finished" << endl;
}

int main()
{
	try
	{
		DoTest( );
	}
	catch (...)
	{
		wcout << endl << L"+++ Error in Win32_utilities_test" << endl;
		return 1;
	}

    return 0;
}
