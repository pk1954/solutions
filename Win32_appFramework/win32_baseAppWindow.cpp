// win32_baseAppWindow.cpp
//
// Win32_appFramework


#include "stdafx.h"
#include "win32_util.h"
#include "win32_modelWindow.h"
#include "win32_histWindow.h"
#include "win32_status.h"
#include "win32_baseAppWindow.h"

void BaseAppWindow::AdjustChildWindows
(
	ModelWindow * pModelWindow,
	HistWindow  * pHistWindow,
	StatusBar   * pStatusBar
)
{
	static PIXEL const HIST_WINDOW_HEIGHT = 30_PIXEL;

	HWND hwndApp = GetParent( pModelWindow->GetWindowHandle() );

	PixelRectSize pntAppClientSize( Util::GetClRectSize( hwndApp ) );
	PIXEL pixAppClientWinWidth  = pntAppClientSize.GetX();
	PIXEL pixAppClientWinHeight = pntAppClientSize.GetY();

	if ( pntAppClientSize.IsNotZero( ) )
	{
		pStatusBar->Resize( );
		pixAppClientWinHeight -= pStatusBar->GetHeight( );
		pixAppClientWinHeight -= HIST_WINDOW_HEIGHT, 
		pHistWindow->Move   // adapt history window to new size
		( 
			0_PIXEL, 
			pixAppClientWinHeight, 
			pixAppClientWinWidth, 
			HIST_WINDOW_HEIGHT, 
			TRUE 
		); 
		pModelWindow->Move
		( 
			0_PIXEL, 
			0_PIXEL, 
			pixAppClientWinWidth, 
			pixAppClientWinHeight, 
			TRUE 
		);
	}
}

