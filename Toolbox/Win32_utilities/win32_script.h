// win32_script.h : 
//
// Win32_utilities

#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <shobjidl.h> 
#include "symtab.h"

using std::wstring;

enum class tFileMode
{
	read,
	write
};

class ScriptFile
{
public:
	ScriptFile();
	~ScriptFile();

	static wstring const GetPathOfExecutable( );
	static wstring const AskForFileName( wstring const, wstring const, tFileMode const );

private:
	static bool          m_bInitialized;
	static IFileDialog * m_pFileDlgOpen;
	static IFileDialog * m_pFileDlgSave;

	static wstring getResult( IFileDialog * const );
};
