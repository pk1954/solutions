// win32_baseDialog.h : 
//

#pragma once

#include "win32_rootWindow.h"

class BaseDialog: public RootWindow
{
public:

    BaseDialog( );

    HWND StartBaseDialog( HWND const, LPCTSTR const, std::function<bool()> const );

	virtual void AddContextMenuEntries( HMENU const, POINT const ) {}

private:
	tOnOffAuto            m_visibilityMode;
	std::function<bool()> m_visibilityCriterion;

	void addWinMenu( HMENU const, std::wstring const ) const;
	void adjustWinMenu( HMENU const ) const;
	void contextMenu( LPARAM );

    virtual INT_PTR UserProc( UINT const, WPARAM const, LPARAM const ) = 0;
		
	void adjustVisibility( tOnOffAuto const onOffAuto )
	{
		if ( m_visibilityCriterion )
			Show( ApplyAutoCriterion( onOffAuto, m_visibilityCriterion ) );
	}

	friend static INT_PTR CALLBACK BaseDialogProc( HWND const, UINT const, WPARAM const, LPARAM const );
};
