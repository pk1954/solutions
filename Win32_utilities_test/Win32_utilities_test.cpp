// Win32_utilities_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "script.h"
#include "win32_winManager.h"

void DoTest( )
{
//	WinManager winManager;


	std::wcout << std::endl << L"*** Win32_utilities_test finished" << std::endl;
}

int main()
{
	try
	{
		DoTest( );
	}
	catch (...)
	{
		std::wcout << std::endl << L"+++ Error in Win32_utilities_test" << std::endl;
		return 1;
	}

    return 0;
}
