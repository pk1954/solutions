// win32_messagePump.cpp
//
// win32_utilities

#include "stdafx.h"
#include "win32_messagePump.h"

int MessagePump
( 
	HINSTANCE const hInstance,
	HWND      const hwndApp,
	int       const iAppResourceId
)
{
	HACCEL const hAccelTable = LoadAccelerators( hInstance, MAKEINTRESOURCE(iAppResourceId) );
	MSG          msg;
	BOOL         bRet;

	while ( bRet = GetMessage( &msg, nullptr, 0, 0 ) != 0 )    // Main message loop
	{
		assert( bRet != -1 );
		if ( !TranslateAccelerator( hwndApp, hAccelTable, &msg ) )
		{
			(void)TranslateMessage( &msg );
			(void)DispatchMessage( &msg );
		}
	}

	return (int) msg.wParam;
}
