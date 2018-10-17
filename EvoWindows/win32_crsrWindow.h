// win32_crsrWindow.h 
//

#pragma once

#include "win32_textWindow.h"

class FocusPoint;
class EvolutionCore;

class CrsrWindow: public TextWindow
{
public:
    CrsrWindow( );

    void Start
	( 
		HWND                  const, 
		FocusPoint          * const, 
		EvolutionCore const * const 
	);

    virtual void DoPaint( TextBuffer & );

private:
    FocusPoint          * m_pFocusPoint;
    EvolutionCore const * m_pCore;
};
