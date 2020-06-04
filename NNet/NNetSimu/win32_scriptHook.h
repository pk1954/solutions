// win32_scriptHook.h
//
// NNetSimu

#pragma once

#include "win32_status.h"
#include "script.h"

class ScriptHook : public Script_Functor
{
public:
	ScriptHook() { }

	void Initialize( StatusBar * const pStatusBar, int iPart, HWND const hwndApp )
	{
		m_pStatusBar     = pStatusBar;
		m_iStatusBarPart = iPart;
		m_hwndApp        = hwndApp;
		m_pStatusBar->AddCustomControl( 80 );  // nr of characters
	}

	virtual void operator( ) ( Script & script ) const
	{
		SendMessage( m_hwndApp, WM_COMMAND, IDM_SCRIPT_PROGRESS, reinterpret_cast<LPARAM>( & script ) );
	}

	void DisplayScriptProgress( Script & script )
	{
		if ( ( m_pStatusBar != nullptr ) && ( script.IsActive() ) )
		{
			wstring   const & wszPath      { script.GetActPath( ) };
			long long const   llFilePos    { script.GetFilePos() };
			uintmax_t const   fileSize     { script.GetFileSize() };
			long      const   lPercentRead { CastToLong( llFilePos * 100 / fileSize ) };
			m_pStatusBar->DisplayInPart( m_iStatusBarPart, L"Reading " + wszPath + L" ... " + std::to_wstring( lPercentRead ) + L"%"  );
		}
	}

private:

	HWND        m_hwndApp        { nullptr };
    StatusBar * m_pStatusBar     { nullptr };
	int         m_iStatusBarPart { 0 };
};
