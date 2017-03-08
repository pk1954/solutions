// win32_rootWindow.cpp
//

#include "stdafx.h"
#include "Windowsx.h"
#include "win32_util.h"
#include "win32_rootWindow.h"

BOOL RootWinIsReady( RootWindow const * pRootWin )
{
    return ( ( pRootWin != nullptr ) && ( pRootWin->GetWindowHandle( ) != nullptr ) );
}

PixelPoint const RootWindow::GetCrsrPosFromLparam( LPARAM const lParam ) const
{
    PixelPoint ptCrsr( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) );
    LONG const lHeight = GetClientWindowHeight( );
    ptCrsr.y = lHeight - ptCrsr.y;
    return ptCrsr;
}

