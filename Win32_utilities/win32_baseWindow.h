// win32_baseWindow.h : 
//

#pragma once

#include "PixelTypes.h"
#include "win32_rootWindow.h"

class BaseWindow : public RootWindow
{
public:

    BaseWindow( );
	~BaseWindow( );

    HWND StartBaseWindow
	( 
		HWND    const hwndParent,
		UINT    const uiClassStyle,
		LPCTSTR const szClass,
		DWORD   const dwWindowStyle,
		PIXEL_X	const xPos   = PIXEL_X(PIXEL(CW_USEDEFAULT)), 
		PIXEL_Y	const yPos 	 = PIXEL_Y(PIXEL(CW_USEDEFAULT)), 
		PIXEL_X	const width  = PIXEL_X(PIXEL(CW_USEDEFAULT)),
		PIXEL_Y	const height = PIXEL_Y(PIXEL(CW_USEDEFAULT))
	);

	virtual void AddContextMenuEntries( HMENU const, POINT const ) {}

private:
    LPCTSTR m_szClass;	

	void contextMenu( LPARAM );
	
	virtual LRESULT UserProc( UINT const, WPARAM const, LPARAM const ) = 0;
	
friend static LRESULT CALLBACK BaseWndProc( HWND const, UINT const, WPARAM const, LPARAM const );
};
