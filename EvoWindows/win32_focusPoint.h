// win32_focusPoint.h
//

#pragma once

#include "GridPoint.h"
#include "HistoryGeneration.h"

class RootWindow;
class ViewCollection;
class EvoHistorySys;
class EvolutionModelData;

class FocusPoint
{
public:
    FocusPoint( );
    virtual ~FocusPoint( );

    void Start( EvoHistorySys *, EvolutionModelData * );

    void SetFocusPoint( GridPoint const );

    void AttachFocusPointObserver( RootWindow const *, INT const );
    
    GridPoint       const GetGridPoint( ) const; 
    HIST_GENERATION const GetGenBirth( )  const; 
    HIST_GENERATION const GetGenDeath( )  const; 
    BOOL            const IsInGrid( )     const; 
    BOOL            const IsAlive( )      const; 
    BOOL            const IsDead( )       const; 
    BOOL            const IsDefined( )    const; 

private:
    EvoHistorySys      * m_pEvoHistorySys;
    EvolutionModelData * m_pModelWork;
    ViewCollection     * m_pViewCol;
    HIST_GENERATION      m_genBirth;
    HIST_GENERATION      m_genDeath;
    GridPoint            m_gp;
};
