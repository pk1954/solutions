// ScriptHook.ixx
//
// Win32_utilities

export module ScriptHook;

import std;
import StatusBar;
import RunTime;

using std::wstring;
using std::to_wstring;

export class ScriptHook : public ScriptFunctor
{
public:
	void Initialize
	(
		StatusBar * const pStatusBar, 
		int         const iPart
	)
	{
		m_pStatusBar     = pStatusBar;
		m_iStatusBarPart = iPart;
		m_pStatusBar->AddCustomControl(80);  // nr of characters
	}

	void operator() (Script & script) const final
	{
		if (!m_pStatusBar)
			return;

		if (Script const * const pScript { ScriptStack::GetScript() })
		{
			wstring const & wszPath       = script.GetActPath();
			wstring const & wszLine       = script.GetActLine();
			int     const   iLineNr       = script.GetActLineNr();
			wstring const   wszScriptLine = wszPath + L"(" + to_wstring(iLineNr) + L"): " + wszLine;
			m_pStatusBar->DisplayInPart(m_iStatusBarPart, wszScriptLine);
		}
		else
		{
			m_pStatusBar->ClearPart(m_iStatusBarPart);
		}
	}

private:
	StatusBar * m_pStatusBar     { nullptr };
	int         m_iStatusBarPart { 0 };
};
