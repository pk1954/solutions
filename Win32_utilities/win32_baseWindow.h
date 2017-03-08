// win32_baseWindow.h : 
//

#pragma once

#include "win32_rootWindow.h"

class BaseWindow : public RootWindow
{
public:

    BaseWindow( );

    HWND StartBaseWindow( HWND const, UINT const, LPCTSTR const, DWORD const );

    virtual LRESULT UserProc( UINT const, WPARAM const, LPARAM const ) = 0;

private:
 
friend static LRESULT CALLBACK BaseWndProc( HWND const, UINT const, WPARAM const, LPARAM const );
};
