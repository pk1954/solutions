// win32_evoHistWindow.h : 
//

#pragma once

#include "win32_histWindow.h"

class EvolutionCore;
class FocusPoint;
class EvoNextGenFunctor;
class EvoHistorySys;
class HistWorkThread;

class EvoHistWindow : public HistWindow
{
public:
    EvoHistWindow( );
    ~EvoHistWindow( );

    void Start( HWND const, FocusPoint * const, EvoHistorySys * const, HistWorkThread * const );

	virtual void PostGotoGeneration( HIST_GENERATION const gen ) 
	{ 
		m_pHistWorkThread->PostGotoGeneration( gen ); 
	}

	virtual void DoPaint( HDC const );

private:
    HistWorkThread * m_pHistWorkThread;
    FocusPoint     * m_pFocusPoint;
};
