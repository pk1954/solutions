// win32_baseWindow.h : 
//

#pragma once

#include "win32_rootWindow.h"

class BaseWindow : public RootWindow
{
public:

    BaseWindow( );
	~BaseWindow( );

    HWND StartBaseWindow( HWND const, int const, int const, int const, int const, UINT const, LPCTSTR const, DWORD const );

	virtual void AddContextMenuEntries( HMENU ) {}

private:
    LPCTSTR m_szClass;	

	void contextMenu( LPARAM );
	
	virtual LRESULT UserProc( UINT const, WPARAM const, LPARAM const ) = 0;
	
friend static LRESULT CALLBACK BaseWndProc( HWND const, UINT const, WPARAM const, LPARAM const );
};
