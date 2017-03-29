// Evolution.cpp
//

//lint -esym( 766, Evolution\Resource.h )    header file not used. lint is wrong here

#include "stdafx.h"
#include "commctrl.h"
#include "win32_util.h"
#include "win32_appWindow.h"
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

	Util::StdOutConsole( );
    InitCommonControls( ); // loads common control's DLL 
    
    AppWindow App;
    App.Start( hInstance, lpCmdLine );

//lint -e1924  C-style cast

    HACCEL const hAccelTable = LoadAccelerators( hInstance, MAKEINTRESOURCE(IDC_EVOLUTION_MAIN) );
    HWND         hwndApp     = App.GetWindowHandle( );
    MSG          msg;

    while ( BOOL const bRet = GetMessage( &msg, nullptr, 0, 0 ) )    // Main message loop
    {
        assert( bRet );
        if ( !TranslateAccelerator( hwndApp, hAccelTable, &msg ) )
        {
            TranslateMessage( &msg );
            //lint -e534  ignore return code
            DispatchMessage( &msg );
        }
    }

    return (int) msg.wParam;
}

