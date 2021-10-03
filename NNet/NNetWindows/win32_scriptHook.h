// win32_scriptHook.h
//
// NNetSimu

#pragma once

#include "win32_status.h"
#include "ScriptStack.h"
#include "script.h"

using std::to_wstring;

class ScriptHook : public ScriptFunctor
{
public:
	ScriptHook() {}

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

	virtual void operator() (Script & script) const
	{
		if (!m_pStatusBar)
			return;

		if (Script * const pScript { ScriptStack::GetScript() })
		{
			m_pStatusBar->DisplayInPart
			(
				m_iStatusBarPart, 
				pScript->GetActPath() + 
				L"(" + 
				to_wstring(pScript->GetActLineNr()) + 
				L"//" + 
				to_wstring(pScript->GetFileSize()) + 
				L"): " + 
				pScript->GetActLine()
			);
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
