// win32_modelWindow.h : 
//
// Win32_appFramework

#pragma once

#include "win32_baseWindow.h"
#include "win32_actionTimer.h"

class ActionTimer;

class ModelWindow : public BaseWindow
{
public:
	ModelWindow( ) :
		BaseWindow( )
	{

	}

	static void InitClass
	( 
		ActionTimer * const pActionTimer
	)
	{
		m_pDisplayTimer = pActionTimer;
		m_hCrsrArrow    = LoadCursor( NULL, IDC_ARROW );
		m_hCrsrMove     = LoadCursor( NULL, IDC_SIZEALL );
	}

	void Refresh( )
	{
		Trigger( );
	}

protected:

	virtual void OnMouseWheel ( WPARAM const, LPARAM const ) = 0;
	virtual void OnMouseMove  ( WPARAM const, LPARAM const ) = 0;
	virtual BOOL OnCommand    ( WPARAM const, LPARAM const ) = 0;
	virtual void OnLButtonDown( WPARAM const, LPARAM const ) = 0;
	virtual void OnLButtonUp  ( WPARAM const, LPARAM const ) = 0;
	virtual void OnSetCursor  ( WPARAM const, LPARAM const ) = 0;
	virtual void OnPaint      ( )                            = 0;

	static ActionTimer * m_pDisplayTimer;
	static HCURSOR       m_hCrsrMove;
	static HCURSOR       m_hCrsrArrow;

private:

	virtual LRESULT UserProc( UINT const, WPARAM const, LPARAM const );

};