// NNetInputOutputUI.ixx
//
// NNetWindows

module;

#include <memory>
#include <cassert>
#include <iostream>
#include <Windows.h>

export module NNetWin32:NNetInputOutputUI;

import InputOutputUI;
import WinManager;
import Win32_Util_Resource;

using std::unique_ptr;
using std::make_unique;
using std::bit_cast;
using std::wstring;

export class NNetInputOutputUI : public InputOutputUI
{
public:
	static unique_ptr<NNetInputOutputUI> CreateNew(int const msg)
	{
		return make_unique<NNetInputOutputUI>(msg);
	}

	explicit NNetInputOutputUI(int const msg)
		:	m_msgImportFinished(msg)
	{}

	void JobFinished(InputOutputUI::Result const res, wstring const & name) const final
	{
		switch (res)
		{
			using enum InputOutputUI::Result;
		case ok:     	   WinManager::SendCommand2App(m_msgImportFinished, 0); break;
		case fileNotFound: WinManager::SendCommand2App(IDX_FILE_NOT_FOUND,  0); break;
		case errorInFile:  WinManager::SendCommand2App(IDX_ERROR_IN_FILE,   0); break;
		default: assert(false);
		}
	};

	void ReadProgressReport(Script * pScript) final
	{ 
		WinManager::SendCommand2App(IDX_READ_PROGRESS_REPORT, bit_cast<LPARAM>(pScript));
	};

	void WriteProgressReport(wstring const & msg) final
	{ 
		WinManager::SendCommand2App(IDX_WRITE_PROGRESS_REPORT, bit_cast<LPARAM>(&msg));
	};

private:
	int m_msgImportFinished;
};
