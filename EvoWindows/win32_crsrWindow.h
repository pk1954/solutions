// win32_crsrWindow.h 
//

#pragma once

#include "win32_textWindow.h"

class FocusPoint;
class EvolutionModelData;

class CrsrWindow: public TextWindow
{
public:
    CrsrWindow( );

    void Start( HWND const, FocusPoint * const, EvolutionModelData const * const );

    virtual void DoPaint( );

private:
    FocusPoint      * m_pFocusPoint;
    EvolutionModelData const * m_pModelWork;
};
