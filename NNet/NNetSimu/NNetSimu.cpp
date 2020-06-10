// NNetSimu.cpp
//
// NNetSimu

#include "stdafx.h"
#include "Resource.h"
#include "commctrl.h"
#include "win32_util.h"
#include "win32_fatalError.h"
#include "win32_stopwatch.h"
#include "win32_messagePump.h"
#include "win32_NNetAppWindow.h"

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

	Util::StdOutConsole( );

	NNetAppWindow App;

	// SetThreadAffinityMask( GetCurrentThread( ), 0x0001 );

	INITCOMMONCONTROLSEX icex // load common control's DLL 
	{
		sizeof( INITCOMMONCONTROLSEX ),
		ICC_STANDARD_CLASSES | 
		ICC_BAR_CLASSES | 
		ICC_TAB_CLASSES | 
		ICC_TREEVIEW_CLASSES  // for tooltips
	};

	int iRetVal;

	try
	{
		Stopwatch stopwatch;
		stopwatch.Start();
		App.Start( );
		stopwatch.Stop( L"App.Start" );

		iRetVal = MessagePump( hInstance, App.GetWindowHandle(), IDC_NNET_SIMU_MAIN );
	}
	catch ( std::logic_error & err )
	{
		FatalError::Happened( 999, err.what() );
	}
	catch ( ... )
	{
		App.Stop();
		FatalError::Happened( 1, "main thread" );
	}

	return iRetVal;
}
