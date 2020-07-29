// win32_script.h : 
//
// Win32_utilities

#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include "symtab.h"

using std::wstring;

enum class tFileMode
{
	read,
	write
};

extern wstring const ScriptDialog( );
extern wstring const GetPathOfExecutable( );
extern wstring const AskForFileName( wstring, wstring const, wstring const, tFileMode const );
