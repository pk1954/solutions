// win32_messagePump.cpp
//
// win32_utilities

#include "stdafx.h"
#include "win32_messagePump.h"

void MessagePump::InstallAccelTable( HWND hwnd, HACCEL haccel )
{
	if ( haccel == nullptr )
		haccel = m_defaultAccelTable;
	m_accEntries.push_back( AccEntry{ hwnd, haccel } );
}

bool MessagePump::translateAcceleratorForWindow( AccEntry const & accEntry, MSG & msg )
{
	if ( accEntry.m_hwnd == msg.hwnd || IsChild( accEntry.m_hwnd, msg.hwnd ) ) 
	{
		if ( TranslateAccelerator( accEntry.m_hwnd, accEntry.m_hAccelTable, & msg ) )
			return true;
		//if ( IsDialogMessage( accEntry.m_hwnd, & msg ) )
		//	return true;
	} 
	return false;
}

int MessagePump::Run( )
{
	MSG  msg;
	BOOL bRet;

	while ( bRet = GetMessage( &msg, nullptr, 0, 0 ) != 0 )    // Main message loop
	{
		assert( bRet != -1 );
		bool bDone { false };
		for ( auto entry : m_accEntries )
		{
			if ( translateAcceleratorForWindow( entry, msg ) )
			{
				bDone = true;
				break;
			}
		}
		if ( ! bDone )
		{
			(void)TranslateMessage( &msg );
			(void)DispatchMessage( &msg );
		}
	}

	return (int) msg.wParam;
}
