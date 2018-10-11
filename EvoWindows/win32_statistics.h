// win32_statistics.h 
//

#pragma once

#include "win32_textWindow.h"
#include "win32_util.h"

class GridRect;
class EvolutionCore;

//lint -esym( 1712, StatisticsWindow )  no default constructor for class 

class StatisticsWindow : public TextWindow
{
public:
    StatisticsWindow( );
    virtual ~StatisticsWindow( );

    void Start( HWND const, EvolutionCore const * const );

    virtual void DoPaint();

private:
    EvolutionCore const * m_pCore;
};
