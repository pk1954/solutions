// win32_monitorWindow.h : 
//
// NNetWindows

#pragma once

#include "D2D_DrawContext.h"
#include "win32_baseWindow.h"

class MonitorWindow : public BaseWindow
{
public:
	void Start( HWND const );
	void Stop( );

	virtual void OnMouseWheel ( WPARAM const, LPARAM const ) { };
	virtual bool OnRButtonUp  ( WPARAM const, LPARAM const ) { return false; }
	virtual bool OnRButtonDown( WPARAM const, LPARAM const ) { return false; }
	virtual void OnLButtonUp  ( WPARAM const, LPARAM const ) { };
	virtual void OnSetCursor  ( WPARAM const, LPARAM const ) { };
	virtual void OnChar       ( WPARAM const, LPARAM const ) { };
	virtual void OnMouseMove  ( WPARAM const, LPARAM const ) { };

	virtual long AddContextMenuEntries( HMENU const ) { return 0; }

	virtual void Notify( bool const ) { };

private:

	virtual void OnPaint ( );

	virtual void doPaint( );

	D2D_DrawContext   m_context     { };
};
