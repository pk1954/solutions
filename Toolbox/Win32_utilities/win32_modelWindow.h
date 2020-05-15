// win32_modelWindow.h : 
//
// Win32_utilities

#pragma once

#include "win32_baseWindow.h"
#include "win32_actionTimer.h"

class ActionTimer;

class ModelWindow : public BaseWindow
{
public:
	ModelWindow( ) :
		BaseWindow( )
	{ }

	static void InitClass( ActionTimer * const pActionTimer	)
	{
		m_pDisplayTimer = pActionTimer;
		m_hCrsrArrow    = LoadCursor( NULL, IDC_ARROW );
		m_hCrsrMove     = LoadCursor( NULL, IDC_HAND );
	}

protected:

	virtual void OnLeftButtonDblClick( WPARAM const, LPARAM const ) = 0;
	virtual void OnMouseWheel        ( WPARAM const, LPARAM const ) = 0;
	virtual void OnMouseMove         ( WPARAM const, LPARAM const ) = 0;
	virtual bool OnCommand           ( WPARAM const, LPARAM const ) = 0;
	virtual void OnLButtonDown       ( WPARAM const, LPARAM const ) = 0;
	virtual bool OnRButtonDown       ( WPARAM const, LPARAM const ) = 0;
	virtual void OnLButtonUp         ( WPARAM const, LPARAM const ) = 0;
	virtual bool OnRButtonUp         ( WPARAM const, LPARAM const ) = 0;
	virtual void OnSetCursor         ( WPARAM const, LPARAM const ) = 0;
	virtual void OnSize              ( WPARAM const, LPARAM const ) = 0;
	virtual void OnPaint             ( )                            = 0;

	inline static ActionTimer * m_pDisplayTimer { nullptr };
	inline static HCURSOR       m_hCrsrMove     { nullptr };
	inline static HCURSOR       m_hCrsrArrow    { nullptr };

private:

	virtual LRESULT UserProc( UINT const, WPARAM const, LPARAM const );
};