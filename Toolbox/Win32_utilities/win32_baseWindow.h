// win32_baseWindow.h : 
//
// win32_utilities

#pragma once

#include "PixelTypes.h"
#include "win32_rootWindow.h"

class BaseWindow : public RootWindow
{
public:

    HWND StartBaseWindow
	( 
		HWND             const, 
		UINT             const, 
		LPCTSTR          const, 
		DWORD            const, 
		PixelRect        const *,
		function<bool()> const
	);

protected:
	virtual bool UserProc( UINT const, WPARAM const, LPARAM const );

private:
	
	friend static LRESULT CALLBACK BaseWndProc( HWND const, UINT const, WPARAM const, LPARAM const );
};
