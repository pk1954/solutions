// win32_baseDialog.h : 
//

#pragma once

#include "win32_rootWindow.h"

class BaseDialog: public RootWindow
{
public:

    BaseDialog( );

    HWND StartBaseDialog( HWND const, LPCTSTR const );

private:

    virtual INT_PTR UserProc( UINT const, WPARAM const, LPARAM const ) = 0;

friend static INT_PTR CALLBACK BaseDialogProc( HWND const, UINT const, WPARAM const, LPARAM const );
};
