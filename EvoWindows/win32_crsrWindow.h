// win32_crsrWindow.h 
//

#pragma once

#include "win32_textWindow.h"

class FocusPoint;
class EvolutionCore;
class GridWindow;            // ++++++++++ EXPERIMENTAL ++++++++++++

class CrsrWindow: public TextWindow
{
public:
    CrsrWindow( );

    void Start
	( 
		HWND                  const, 
		FocusPoint          * const, 
		EvolutionCore const * const, 
		GridWindow    const * const 
	);

    virtual void DoPaint( );

private:
    FocusPoint          * m_pFocusPoint;
    EvolutionCore const * m_pCore;
	GridWindow    const * m_pGridWindow;
};
