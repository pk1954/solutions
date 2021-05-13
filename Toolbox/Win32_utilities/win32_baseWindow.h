// win32_baseWindow.h : 
//
// win32_utilities

#pragma once

#include "PixelTypes.h"
#include "win32_rootWindow.h"

static LRESULT CALLBACK BaseWndProc( HWND const, UINT const, WPARAM const, LPARAM const );

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

	virtual void OnLeftButtonDblClick( WPARAM const, LPARAM const ) {};
	virtual void OnMouseWheel        ( WPARAM const, LPARAM const ) {};
	virtual void OnMouseMove         ( WPARAM const, LPARAM const ) {};
	virtual void OnChar              ( WPARAM const, LPARAM const ) {};
	virtual void OnLButtonDown       ( WPARAM const, LPARAM const ) {};
	virtual void OnLButtonUp         ( WPARAM const, LPARAM const ) {};
	virtual bool OnRButtonDown       ( WPARAM const, LPARAM const ) { return false; };
	virtual bool OnRButtonUp         ( WPARAM const, LPARAM const ) { return false; };
	virtual bool OnSize              ( WPARAM const, LPARAM const ) { return false; };
	virtual bool OnShow              ( WPARAM const, LPARAM const ) { return false; };
	virtual bool OnMouseLeave        ( WPARAM const, LPARAM const ) { return false; };
	virtual bool OnSetCursor         ( WPARAM const, LPARAM const ) { return false; };
	virtual bool OnTimer             ( WPARAM const, LPARAM const ) { return false; };

	virtual void OnPaint() = 0;

private:
	
	friend static LRESULT CALLBACK BaseWndProc( HWND const, UINT const, WPARAM const, LPARAM const );
};
