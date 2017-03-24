#pragma once

#include "GridPoint.h"
#include "HistoryGeneration.h"
#include "EvolutionModelData.h"
#include "win32_viewCollection.h"

class RootWindow;
class EvoHistorySys;

class FocusPoint
{
public:
    FocusPoint( ) :
        m_pEvoHistorySys( nullptr ),
        m_pModelWork( nullptr ),
        m_ViewCol( ),
        m_gp( ),
        m_genBirth( - 1 ),
        m_genDeath( - 1 )
    { };

    virtual ~FocusPoint( ) { };

    void Start( EvoHistorySys *, EvolutionModelData * );

    void SetGridPoint( GridPoint const );

    void AttachFocusPointObserver( RootWindow const *, INT const );
    
    GridPoint       const GetGridPoint( ) const { return m_gp; }
    HIST_GENERATION const GetGenBirth( )  const { return m_genBirth; }
    HIST_GENERATION const GetGenDeath( )  const { return m_genDeath; }
    BOOL            const IsInGrid( )     const { return m_gp.IsInGrid( ); }
    BOOL            const IsAlive( )      const { return m_pModelWork->IsAlive( m_gp ); }
    BOOL            const IsDead( )       const { return m_pModelWork->IsDead ( m_gp ); }

private:
    EvoHistorySys      * m_pEvoHistorySys;
    EvolutionModelData * m_pModelWork;
    ViewCollection       m_ViewCol;
    GridPoint            m_gp;
    HIST_GENERATION      m_genBirth;
    HIST_GENERATION      m_genDeath;
};
