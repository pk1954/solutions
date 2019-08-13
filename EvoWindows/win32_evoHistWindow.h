// win32_evoHistWindow.h : 
//

#pragma once

#include "win32_histWindow.h"

class FocusPoint;
class HistorySystem;
class WorkThreadInterface;

class EvoHistWindow : public HistWindow
{
public:
    EvoHistWindow( );
	virtual ~EvoHistWindow( ) { };

	void Start
	( 
		HWND                  const, 
		FocusPoint          * const, 
		HistorySystem       * const, 
		WorkThreadInterface * const 
	);

	void Stop( );

	virtual void AddContextMenuEntries( HMENU const, POINT const );
	virtual void GotoGeneration( HIST_GENERATION const );
	virtual void DoPaint( HDC const );

private:
    WorkThreadInterface * m_pWorkThreadInterface;
    FocusPoint          * m_pFocusPoint;
};
