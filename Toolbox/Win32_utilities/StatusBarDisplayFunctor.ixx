// StatusBarDisplayFunctor.ixx
//
// Win32_utilities

module;

#include <string>

export module StatusBarDisplayFunctor;

import StatusBar;
import DisplayFunctor;

using std::wstring;

export class StatusBarDisplayFunctor : public DisplayFunctor
{
public:
	void Initialize(StatusBar* const pStatusBar, int const iPart)
	{
		m_pStatusBar = pStatusBar;
		m_iPart = iPart;
	}

	void operator() (wstring const& line) final
	{
		if (m_pStatusBar)
		{
			m_wstring = line;
			m_pStatusBar->DisplayInPart(m_iPart, m_wstring);
		}
	}

private:
	wstring     m_wstring    { };
	StatusBar * m_pStatusBar { nullptr };
	int         m_iPart      { 0 };
};
