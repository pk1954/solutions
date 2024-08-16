// win32_scriptHook.h
//
// win32_appFramework

module;

import RunTime;
import StatusBar;

class ScriptHook : public Script_Functor
{
public:
	ScriptHook()
	:  m_pStatusBar(nullptr),
	   m_iStatusBarPart(0)
	{}

	void Initialize(StatusBar * const pStatusBar, int iPart)
	{
		m_pStatusBar = pStatusBar;
		m_iStatusBarPart = iPart;
	}

    virtual void operator() (Script & script) const
    {
        if (m_pStatusBar != nullptr)
        {
			wstring m_wstrScriptLine;

			if (script.IsActive())
			{
				wstring const & wszPath = script.GetActPath();
				wstring const & wszLine = script.GetActLine();
				int     const   iLineNr = script.GetActLineNr();
				m_wstrScriptLine = wszPath + L"(" + std::to_wstring(iLineNr) + L"): " + wszLine;
			}
			else 
			{
				m_wstrScriptLine = L"";
			}

			m_pStatusBar->DisplayInPart(m_iStatusBarPart, m_wstrScriptLine);
		}
    }

private:

    StatusBar * m_pStatusBar;
	int         m_iStatusBarPart;
};
