// NNetInputOutputUI.ixx
//
// NNetWindows

module;

#include <memory>
#include <cassert>
#include <iostream>
#include <Windows.h>
#include "Resource.h"

export module NNetWin32:NNetInputOutputUI;

import InputOutputUI;

using std::unique_ptr;
using std::make_unique;
using std::bit_cast;
using std::wstring;

export class NNetInputOutputUI : public InputOutputUI
{
public:
	static void Initialize(HWND const hwndApp)
	{
		m_hwndApp = hwndApp;
	}

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
		case ok:     	   SendMessage(m_hwndApp, WM_COMMAND, m_msgImportFinished, 0); break;
		case fileNotFound: SendMessage(m_hwndApp, WM_COMMAND, IDX_FILE_NOT_FOUND,  0); break;
		case errorInFile:  SendMessage(m_hwndApp, WM_COMMAND, IDX_ERROR_IN_FILE,   0); break;
		default: assert(false);
		}
	};

	void ReadProgressReport(Script * pScript) final
	{ 
		SendMessage(m_hwndApp, WM_COMMAND, IDX_READ_PROGRESS_REPORT, bit_cast<LPARAM>(pScript));
	};

	void WriteProgressReport(wstring const & msg) final
	{ 
		SendMessage(m_hwndApp, WM_COMMAND, IDX_WRITE_PROGRESS_REPORT, bit_cast<LPARAM>(&msg));
	};

private:
	inline static HWND m_hwndApp;

	int m_msgImportFinished;
};
