// win32_script.h : 
//
// Win32_utilities

#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <ShObjIdl.h> 
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

	static wstring GetPathOfExecutable();
	static wstring AskForFileName(wstring const &, wstring const &, tFileMode const);

private:
	static bool m_bInitialized;

	static IFileDialog * createOpenDialog();
	static IFileDialog * createSaveDialog();

	static bool    setFileTypes       (IFileDialog *, wstring const &, wstring const &);
	static bool    setDefaultExtension(IFileDialog *, wstring const &);
	static wstring getResult(IFileDialog * const);
};
