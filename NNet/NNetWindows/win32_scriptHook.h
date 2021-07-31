// win32_scriptHook.h
//
// NNetSimu

#pragma once

#include "win32_baseRefreshRate.h"
#include "win32_status.h"
#include "script.h"

using std::to_wstring;

class ScriptHook : public Script_Functor
{
public:
	ScriptHook() {}

	void Initialize
	( 
		StatusBar * const pStatusBar, 
		int         const iPart, 
		Script    * const pScript
	)
	{
		m_pStatusBar     = pStatusBar;
		m_iStatusBarPart = iPart;
		m_pScript        = pScript;
		m_pRefreshRate   = new refreshRate( this );
		m_pRefreshRate->SetRefreshRate( 300ms );
		m_pStatusBar->AddCustomControl( 80 );  // nr of characters
	}

	virtual void operator() (Script & script) const
	{
		m_pRefreshRate->Notify( false );
	}

	void DisplayScriptProgress()
	{
		if ( ( m_pStatusBar != nullptr ) && ( m_pScript->IsActive() ) )
		{
			wstring const & wstrPath     { m_pScript->GetActPath () };
			long    const   lPercentRead { m_pScript->GetPercentRead() };
			m_pStatusBar->DisplayInPart
			( 
				m_iStatusBarPart, 
				L"Reading " + wstrPath + L" ... " + to_wstring( lPercentRead ) + L"%"  
			);
		}
		else
		{
			m_pStatusBar->ClearPart(m_iStatusBarPart);
		}
	}

private:
	class refreshRate : public BaseRefreshRate
	{
	public:
		refreshRate( ScriptHook * const pScriptHook )
			: m_pScriptHook( pScriptHook )
		{ }

		virtual void Trigger()
		{
			m_pScriptHook->DisplayScriptProgress();
		}

	private:
		ScriptHook * m_pScriptHook;
	};

	refreshRate * m_pRefreshRate   { nullptr };
	StatusBar   * m_pStatusBar     { nullptr };
	Script      * m_pScript        { nullptr };
	int           m_iStatusBarPart { 0 };
};
