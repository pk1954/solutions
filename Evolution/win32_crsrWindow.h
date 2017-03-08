// win32_crsrWindow.h 
//

#pragma once

#include "win32_textWindow.h"

class FocusPoint;
class ModelData;

class CrsrWindow: public TextWindow
{
public:
    CrsrWindow( );

    void Start( HWND const, FocusPoint * const, ModelData const * const );

    virtual void DoPaint( );

private:
    FocusPoint      * m_pFocusPoint;
    ModelData const * m_pModelWork;
};
