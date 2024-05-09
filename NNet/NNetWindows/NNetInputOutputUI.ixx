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
import RootWindow;
import Win32_Util_Resource;

using std::unique_ptr;
using std::make_unique;
using std::bit_cast;
using std::wstring;

export class NNetInputOutputUI : public InputOutputUI
{
public:
	static unique_ptr<NNetInputOutputUI> CreateNew
	(
		int const msg,
		RootWindow const *pRootWin
	)
	{
		return make_unique<NNetInputOutputUI>(msg, pRootWin);
	}

	explicit NNetInputOutputUI
	(
		int        const  msg,
		RootWindow const *pRootWin
	)
	  :	m_msgImportFinished(msg),
		m_pRootWinInitiator(pRootWin)
	{}

	void JobFinished(InputOutputUI::Result const res, wstring const & name) const final
	{
		switch (res)
		{
			using enum InputOutputUI::Result;
		case ok:     	   m_pRootWinInitiator->SendCommand(m_msgImportFinished, 0); break;
		case fileNotFound: m_pRootWinInitiator->SendCommand(IDX_FILE_NOT_FOUND,  0); break;
		case errorInFile:  m_pRootWinInitiator->SendCommand(IDX_ERROR_IN_FILE,   0); break;
		default: assert(false);
		}
	};

	void ReadProgressReport(Script * pScript) final
	{ 
		m_pRootWinInitiator->SendCommand(IDX_READ_PROGRESS_REPORT, bit_cast<LPARAM>(pScript));
	};

	void WriteProgressReport(wstring const & msg) final
	{ 
		m_pRootWinInitiator->SendCommand(IDX_WRITE_PROGRESS_REPORT, bit_cast<LPARAM>(&msg));
	};

private:
	int                m_msgImportFinished;
	RootWindow const * m_pRootWinInitiator;
};
