// win32_evoHistWindow.h : 
//

#pragma once

#include "win32_histWindow.h"

class EvolutionCore;
class EvolutionModelData;
class FocusPoint;
class EvoNextGenFunctor;
class EvoHistorySys;
class HistWorkThread;

class EvoHistWindow : public HistWindow
{
public:
    EvoHistWindow( );
    ~EvoHistWindow( );

    void Start( HWND const, FocusPoint * const, EvolutionModelData * const, EvoHistorySys * const, HistWorkThread * const );

	virtual void            PostGotoGeneration( HIST_GENERATION const gen ) { m_pHistWorkThread->PostGotoGeneration( gen ); }
	virtual HIST_GENERATION GetGenDemanded    ()            const           { return m_pHistWorkThread->GetGenDemanded( ); }

	virtual void            DoPaint( HDC const );

private:

    void paintLifeLine( HDC const ) const;

    HistWorkThread  * m_pHistWorkThread;
    FocusPoint      * m_pFocusPoint;
    EvoHistorySys   * m_pEvoHistorySys;
};
