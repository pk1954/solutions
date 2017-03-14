// win32_evoHistWindow.h : 
//

#pragma once

#include "win32_histWindow.h"

class EvolutionCore;
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

    void Start( HWND const, wofstream * const, FocusPoint * const, StatusBar * const, EvoNextGenFunctor * const, EvoModelData * const, EvolutionCore * const );

    HistWorkThread * GetHistWorkThread( );

    virtual void            PostGotoGeneration( HIST_GENERATION const gen );
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
