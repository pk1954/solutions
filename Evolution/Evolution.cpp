// Evolution.cpp
//

//lint -esym( 766, Evolution\Resource.h )    header file not used. lint is wrong here

#include "stdafx.h"
#include "commctrl.h"
#include "win32_appWindow.h"
#include "win32_stopwatch.h"
#include "Resource.h"

//lint -e952 some parameters could be declared as const
//lint -e818 some parameters could be declared as pointing to const

// not possible as WinMain has signature defined by windows

//lint -esym( 714, WinMain(struct HINSTANCE__ *, struct HINSTANCE__ *, wchar_t *, int)     not referenced

int APIENTRY _tWinMain
(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR    lpCmdLine,
    int       nCmdShow
)
{
//lint -esym( 550, hPrevInstance )  parameter not used
//lint -esym( 550, nCmdShow )       parameter not used
//lint -esym( 715, nCmdShow )       parameter not used

    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );
    UNREFERENCED_PARAMETER( nCmdShow );

	SetThreadAffinityMask( GetCurrentThread( ), 0x0001 );

    INITCOMMONCONTROLSEX icex; // load common control's DLL 
    icex.dwSize = sizeof( INITCOMMONCONTROLSEX );
    icex.dwICC  = ICC_STANDARD_CLASSES | ICC_BAR_CLASSES | ICC_TAB_CLASSES;
    InitCommonControlsEx(&icex); 
    
 	Stopwatch stopwatch;
	stopwatch.Start();
    AppWindow App;
    App.Start( hInstance, lpCmdLine );
	stopwatch.Stop( L"App.Start" );

//lint -e1924  C-style cast

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

