// Win32_FatalError.ixx
//
// Toolbox\win32_utilities

export module FatalErrorMB;

import std;
import WinBasics;
import FatalError;

using std::wstring;
using std::wostringstream;
using std::endl;

export class FatalErrorMB
{
public:
	static void Happened(long const errnr, wstring const &errText)
	{
		wostringstream wBuffer;
		wBuffer << L"Error number:  " << errnr << endl;
		wBuffer << errText << endl;
		wBuffer << L"Press OK to exit application" << endl;
		MessageBoxW(nullptr, wBuffer.str().c_str(), L"Fatal error", MB_OK|MB_SYSTEMMODAL);
		FatalError::Happened(errnr, errText);
	}
};