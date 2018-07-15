// win32_evoHistWindow.h : 
//

#pragma once

#include "win32_histWindow.h"

class EvolutionCore;
class FocusPoint;
class EvoNextGenFunctor;
class EvoHistorySys;
class WorkThread;

class EvoHistWindow : public HistWindow
{
public:
    EvoHistWindow( );
    virtual ~EvoHistWindow( );

    void Start( HWND const, FocusPoint * const, EvoHistorySys * const, WorkThread * const );

	virtual void GotoGeneration( HIST_GENERATION const gen ) 
	{ 
		m_pWorkThread->PostGotoGeneration( gen ); 
	}

	virtual void DoPaint( HDC const );

private:
    WorkThread * m_pWorkThread;
    FocusPoint * m_pFocusPoint;
};
