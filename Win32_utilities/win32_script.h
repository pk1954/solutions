// win32_script.h : 
//
// WIn32_utilities

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

extern wstring GetPathOfExecutable( );
extern wstring AskForFileName( wstring, wstring const, wstring const, tFileMode const );
