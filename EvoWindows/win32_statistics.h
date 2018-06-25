// win32_statistics.h 
//

#pragma once

#include "win32_textWindow.h"
#include "win32_util.h"

//lint -esym( 763, WorkThread )  redundant declaration
//lint -esym( 763, GridRect )    redundant declaration

class EvoHistorySys;
class GridRect;
class EvolutionModelData;

//lint -esym( 1712, StatisticsWindow )  no default constructor for class 

class StatisticsWindow : public TextWindow
{
public:
    StatisticsWindow( );
    virtual ~StatisticsWindow( );

    void Start( HWND const, EvolutionModelData const * const );

    virtual void DoPaint();

private:
    EvolutionModelData const * m_pModelWork;
};
