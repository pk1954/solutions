// win32_messagePump.cpp
//
// win32_utilities

#include "stdafx.h"
#include "win32_messagePump.h"

void MessagePump::SetAccelTable( HINSTANCE const hInstance, int const idResource )
{
	HACCEL haccel { LoadAccelerators( hInstance, MAKEINTRESOURCE(idResource) ) };
	m_defaultAccelTable = haccel;
}

void MessagePump::RegisterWindow( HWND const hwnd, bool const bIsDialog )
{
	m_accEntries.push_back( AccEntry{ hwnd, bIsDialog } );
}

bool MessagePump::accelerator(  MSG & msg )
{
	for ( auto entry : m_accEntries )
	{
		if ( entry.m_hwnd == msg.hwnd || IsChild( entry.m_hwnd, msg.hwnd ) ) 
		{
			if ( TranslateAccelerator( entry.m_hwnd, m_defaultAccelTable, & msg ) )
				return true;
			if ( entry.m_bIsDialog && IsDialogMessage( entry.m_hwnd, & msg ) )
				return true;
		} 
	}
	return false;
}

int MessagePump::Run( )
{
	MSG  msg;
	BOOL bRet;

	while ( (bRet = GetMessage( &msg, nullptr, 0, 0 )) != 0 )    // Main message loop //-V559 //-V593
	{
		assert( bRet != -1 ); //-V547
		if ( ! accelerator( msg ) )
		{
			(void)TranslateMessage( &msg );
			(void)DispatchMessage( &msg );
		}
	}

	return (int) msg.wParam;
}
