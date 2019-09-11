// win32_NNetHistWindow.h : 
//
// NNetWindows

#pragma once

#include "win32_histWindow.h"

class HistorySystem;
class WorkThreadInterface;

class NNetHistWindow : public HistWindow
{
public:
	NNetHistWindow( );
	virtual ~NNetHistWindow( ) { };

	void Start
	( 
		HWND                  const, 
		HistorySystem       * const, 
		WorkThreadInterface * const 
	);

	void Stop( );

	virtual void GotoGeneration( HIST_GENERATION const );
	virtual void DoPaint( HDC const );

private:
	WorkThreadInterface * m_pWorkThreadInterface;
};
