// win32_statistics.h 
//

#pragma once

#include "win32_textWindow.h"

class EvolutionCore;

class StatisticsWindow : public TextWindow
{
public:
    StatisticsWindow( );
    virtual ~StatisticsWindow( );

    void Start( HWND const, EvolutionCore const * const );

    virtual void DoPaint( TextBuffer & );

private:
    EvolutionCore const * m_pCore;
};
