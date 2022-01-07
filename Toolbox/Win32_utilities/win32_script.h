// win32_script.h : 
//
// Win32_utilities

#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <ShObjidl.h> 
#include "SYMTAB.H"

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

	static wstring const GetPathOfExecutable();
	static wstring const AskForFileName(wstring const, wstring const, tFileMode const);

private:
	static bool m_bInitialized;

	static IFileDialog * createOpenDialog();
	static IFileDialog * createSaveDialog();

	static bool    const setFileTypes       (IFileDialog *, wstring const, wstring const);
	static bool    const setDefaultExtension(IFileDialog *, wstring const);
	static wstring const getResult(IFileDialog * const);
};
