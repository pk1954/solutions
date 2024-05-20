// Win32_Script.ixx
//
// Win32_utilities

module;

#include <string>
#include <fstream>
#include <iostream>
#include <string>
#include <ShObjIdl.h> 

export module ScriptFile;

import Symtab;
import Script;

using std::wstring;

export enum class tFileMode
{
	read,
	write
};

export class ScriptFile
{
public:
	ScriptFile();
	~ScriptFile();

	static wstring GetPathOfExecutable();
	static wstring AskForFileName(wstring const&, wstring const&, wstring const&, tFileMode const);

private:
	static bool m_bInitialized;

	static IFileDialog * createOpenDialog();
	static IFileDialog * createSaveDialog(wstring const &);

	static bool    setFileTypes       (IFileDialog *, wstring const &, wstring const &);
	static bool    setDefaultExtension(IFileDialog *, wstring const &);
	static wstring getResult(IFileDialog * const);
};

export void StartScript(wstring const&, ScriptFunctor const&);
