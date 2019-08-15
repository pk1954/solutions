// win32_baseWindow.h : 
//
// win32_utilities

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
		HWND                  const, 
		UINT                  const, 
		LPCTSTR               const, 
		DWORD                 const, 
		PixelRect             const *,
		std::function<bool()> const
	);

	virtual void AddContextMenuEntries( HMENU const, POINT const ) {}

private:
	tOnOffAuto            m_visibilityMode;
	std::function<bool()> m_visibilityCriterion;

	void addWinMenu( HMENU const, std::wstring const ) const;
	void adjustWinMenu( HMENU const ) const;
	void contextMenu( LPARAM );
	
	virtual LRESULT UserProc( UINT const, WPARAM const, LPARAM const ) = 0;

	void adjustVisibility( tOnOffAuto const onOffAuto )
	{
		if ( m_visibilityCriterion )
			Show( ApplyAutoCriterion( onOffAuto, m_visibilityCriterion ) );
	}
	
	friend static LRESULT CALLBACK BaseWndProc( HWND const, UINT const, WPARAM const, LPARAM const );
};
