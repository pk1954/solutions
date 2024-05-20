// NNetUtilities.ixx
//
// NNetWindows

module;

#include <string>

export module NNetWin32:NNetUtilities;

import ScriptFile;

using std::wstring;

export wstring AskModelFileName(enum class tFileMode const mode)
{
	return ScriptFile::AskForFileName(L"mod", L"", L"Model files", mode);
}

export wstring AskScanFileName(wstring const& wstrName, enum class tFileMode const mode)
{
	return ScriptFile::AskForFileName(L"scan", wstrName, L"Scan files", mode);
}

