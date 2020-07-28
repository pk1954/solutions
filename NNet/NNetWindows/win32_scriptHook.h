// win32_scriptHook.h
//
// NNetSimu

#pragma once

#include "win32_baseRefreshRate.h"
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
		m_pRefreshRate   = new refreshRate( m_hwndApp );
		m_pRefreshRate->SetRefreshRate( 300ms );
		m_pStatusBar->AddCustomControl( 80 );  // nr of characters
	}

	virtual void operator( ) ( Script & script ) const
	{
		m_pRefreshRate->Notify( false );
	}

	void DisplayScriptProgress( Script & script ) // TODO: move function to better place
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
	class refreshRate : public BaseRefreshRate
	{
	public:
		refreshRate( HWND const hwndApp )
			: m_hwndApp( hwndApp )
		{ }

		virtual void Trigger( )
		{
			SendMessage( m_hwndApp, WM_COMMAND, IDM_SCRIPT_PROGRESS, 0 );
		}

	private:
		HWND const m_hwndApp;
	};

	refreshRate * m_pRefreshRate   { nullptr };
    StatusBar   * m_pStatusBar     { nullptr };
	HWND          m_hwndApp        { nullptr };
	int           m_iStatusBarPart { 0 };
};
