// win32_evoHistWindow.h : 
//
// EvoWindows

#pragma once

#include "win32_histWindow.h"

class FocusPoint;
class HistorySystem;
class EvoWorkThreadInterface;

class EvoHistWindow : public HistWindow
{
public:
    EvoHistWindow( );
	virtual ~EvoHistWindow( ) { };

	void Start
	( 
		HWND                     const, 
		FocusPoint             * const, 
		HistorySystem          * const, 
		EvoWorkThreadInterface * const 
	);

	void Stop( );

	virtual void GotoGeneration( HIST_GENERATION const );
	virtual void DoPaint( HDC const );

private:
    EvoWorkThreadInterface * m_pWorkThreadInterface;
    FocusPoint             * m_pFocusPoint;
};
