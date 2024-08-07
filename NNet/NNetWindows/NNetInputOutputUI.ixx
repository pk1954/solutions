// NNetInputOutputUI.ixx
//
// NNetWindows

export module NNetWin32:NNetInputOutputUI;

import std;
import InputOutputUI;
import WinBasics;
import Win32_Util;
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
		int  const msg,
		HWND const hwnd
	)
	{
		return make_unique<NNetInputOutputUI>(msg, hwnd);
	}

	explicit NNetInputOutputUI
	(
		int  const msg,
		HWND const hwnd
	)
	  :	m_msgJobFinished(msg),
		m_hwndInitiator(hwnd)
	{}

	void JobFinished(InputOutputUI::Result const res, wstring const & name) const final
	{
		switch (res)
		{
			using enum InputOutputUI::Result;
		case ok:     	   ::SendCommand(m_hwndInitiator, m_msgJobFinished,   0); break;
		case fileNotFound: ::SendCommand(m_hwndInitiator, IDX_FILE_NOT_FOUND, 0); break;
		case errorInFile:  ::SendCommand(m_hwndInitiator, IDX_ERROR_IN_FILE,  0); break;
		default: ; //assert(false);
		}
	};

	void ReadProgressReport(Script * pScript) final
	{ 
		::SendCommand(m_hwndInitiator, IDX_READ_PROGRESS_REPORT, bit_cast<LPARAM>(pScript));
	};

	void WriteProgressReport(wstring const & msg) final
	{ 
		::SendCommand(m_hwndInitiator, IDX_WRITE_PROGRESS_REPORT, bit_cast<LPARAM>(&msg));
	};

private:
	int  m_msgJobFinished;
	HWND m_hwndInitiator;
};
