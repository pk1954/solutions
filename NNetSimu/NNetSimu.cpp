// NNetSimu.cpp
//
// NNetSimu

#include "stdafx.h"
#include "commctrl.h"
#include "Resource.h"
#include "win32_util.h"
#include "win32_stopwatch.h"
#include "win32_NNetSimuWindow.h"

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
        ICC_STANDARD_CLASSES | 
		ICC_BAR_CLASSES | 
		ICC_TAB_CLASSES | 
		ICC_TREEVIEW_CLASSES  // for tooltips
	};
    InitCommonControlsEx(&icex); 

	Util::StdOutConsole( );

 	Stopwatch stopwatch;
	stopwatch.Start();
	NNetSimuWindow App;
	App.Start( );
	stopwatch.Stop( L"App.Start" );

    HACCEL const hAccelTable = LoadAccelerators( hInstance, MAKEINTRESOURCE(IDC_NNET_SIMU_MAIN) );
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

