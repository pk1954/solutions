// win32_evoHistWindow.h : 
//

#pragma once

#include "win32_histWindow.h"

class EvolutionCore;
class FocusPoint;
class EvoNextGenFunctor;
class EvoHistorySysGlue;
class WorkThreadInterface;

class EvoHistWindow : public HistWindow
{
public:
    EvoHistWindow( );
    virtual ~EvoHistWindow( );

    void Start
	( 
		HWND                  const, 
		FocusPoint          * const, 
		EvoHistorySysGlue   * const, 
		WorkThreadInterface * const 
	);

	virtual void GotoGeneration( HIST_GENERATION const );
	virtual void DoPaint( HDC const );

private:
    WorkThreadInterface * m_pWorkThreadInterface;
    FocusPoint          * m_pFocusPoint;
};
