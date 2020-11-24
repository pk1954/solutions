// win32_NNetAppMenu.h
//
// NNetWindows

#pragma once

#include <string>
#include "ObserverInterface.h"

using std::wstring;

class WinManager;
class CommandStack;
class ComputeThread;

class NNetAppMenu : public ObserverInterface
{
public:
	NNetAppMenu();
	virtual ~NNetAppMenu();

	void Start
	( 
		HWND                  const, 
		ComputeThread const * const, 
		WinManager    const * const,
		CommandStack  const * const,
		Sound         const * const 
	);

	void AdjustUndoRedo( );

	virtual void Notify( bool const );

private:

	class OnOffPair
	{
	public:
		OnOffPair
		( 
			NNetAppMenu      * pMenu,
			unsigned int const iOn, 
			unsigned int const iOff 
		)
		:	m_pAppMenu( pMenu ),
			m_iOn     ( iOn ),
			m_iOff    ( iOff )
		{ }

		void appendOnOffMenu( HMENU hMenu, LPCTSTR const title )
		{
			HMENU hMenuPopup = m_pAppMenu->popupMenu( hMenu, title );
			AppendMenu( hMenuPopup, MF_STRING, m_iOn,  L"o&n" );
			AppendMenu( hMenuPopup, MF_STRING, m_iOff, L"o&ff" );
		}

		void enableOnOff( bool const bCrit )
		{
			m_pAppMenu->enable( m_iOn,    bCrit );
			m_pAppMenu->enable( m_iOff, ! bCrit );
		}

	private:
		NNetAppMenu * m_pAppMenu;
		unsigned int  m_iOn;
		unsigned int  m_iOff;
	};

	HMENU popupMenu( HMENU const, LPCTSTR const );
	void  enable( unsigned int const, bool const );

	HWND                  m_hwndApp        { nullptr };
	HMENU                 m_hMenu          { nullptr };
	ComputeThread const * m_pComputeThread { nullptr };
	WinManager    const * m_pWinManager    { nullptr };
	CommandStack  const * m_pCommandStack  { nullptr };
	Sound         const * m_pSound         { nullptr };

	OnOffPair * m_pOnOffArrows;
	OnOffPair * m_pOnOffSound;
	OnOffPair * m_pOnOffAutoOpen;
};