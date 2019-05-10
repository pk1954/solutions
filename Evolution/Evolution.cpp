// Evolution.cpp
//

//lint -esym( 766, Evolution\Resource.h )    header file not used. lint is wrong here

#include "stdafx.h"
#include "commctrl.h"
#include "win32_appWindow.h"
#include "win32_stopwatch.h"
#include "Resource.h"

int APIENTRY wWinMain
(
	_In_     HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_     LPWSTR    lpCmdLine,
	_In_     int       nCmdShow
)
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );
    UNREFERENCED_PARAMETER( nCmdShow );

//	SetThreadAffinityMask( GetCurrentThread( ), 0x0001 );

    INITCOMMONCONTROLSEX icex // load common control's DLL 
	{
        sizeof( INITCOMMONCONTROLSEX ),
        ICC_STANDARD_CLASSES | ICC_BAR_CLASSES | ICC_TAB_CLASSES
	};
    InitCommonControlsEx(&icex); 
    
 	Stopwatch stopwatch;
	stopwatch.Start();
    AppWindow App;
    App.Start( );
	stopwatch.Stop( L"App.Start" );

    HACCEL const hAccelTable = LoadAccelerators( hInstance, MAKEINTRESOURCE(IDC_EVOLUTION_MAIN) );
    HWND         hwndApp     = App.GetWindowHandle( );
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

