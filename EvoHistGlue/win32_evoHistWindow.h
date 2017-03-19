// win32_evoHistWindow.h : 
//

#pragma once

#include "win32_histWindow.h"

class EvolutionCore;
class EvolutionModelData;
class FocusPoint;
class StatusBar;
class EvoNextGenFunctor;
class EvoHistorySys;
class HistWorkThread;
class HistAllocThread;

class EvoHistWindow : public HistWindow
{
public:
    EvoHistWindow( );
    ~EvoHistWindow( );

    void Start( HWND const, FocusPoint * const, StatusBar * const, EvolutionModelData * const, EvoHistorySys * const, HistWorkThread * const );

	virtual void            PostGotoGeneration( HIST_GENERATION const );
	virtual HIST_GENERATION GetGenDemanded( ) const; 
	virtual void            DoPaint( HDC const );

private:

    void paintLifeLine( HDC const ) const;
    void shutDownHistoryCache( );

    HistWorkThread  * m_pHistWorkThread;
    FocusPoint      * m_pFocusPoint;
    StatusBar       * m_pStatusBar;
    EvoHistorySys   * m_pEvoHistorySys;
	HistAllocThread * m_pHistAllocThread;
};
