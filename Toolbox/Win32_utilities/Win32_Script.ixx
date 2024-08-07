// Win32_Script.ixx
//
// Win32_utilities

module;

#include <ShObjIdl.h> 

export module ScriptFile;

import std;
import RunTime;

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
