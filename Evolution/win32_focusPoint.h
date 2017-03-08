#pragma once

#include "GridPoint.h"
#include "EvoHistorySys.h"
#include "win32_viewCollection.h"

class ModelData;

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

    void Start
    ( 
        EvoHistorySys * pEvoHistorySys,
        ModelData     * pModel
    )
    {
        assert( pEvoHistorySys != nullptr );
        assert( pModel != nullptr );
        m_pEvoHistorySys = pEvoHistorySys;
        m_pModelWork = pModel;
    }

    void SetGridPoint( GridPoint const gpNew )
    {
        if ( gpNew != m_gp )
        {
            m_gp = gpNew;
            if ( IsInGrid( ) )
            {
                if ( m_pEvoHistorySys != nullptr )
                {
                    IndId id = m_pModelWork->GetId( m_gp );
                    m_genBirth = m_pEvoHistorySys->GetFirstGenOfIndividual( id );
                    m_genDeath = m_pEvoHistorySys->GetLastGenOfIndividual ( id ) + 1;
                }
            }
            m_ViewCol.NotifyObservers( );
        }
    }

    void AttachFocusPointObserver( RootWindow const * pRootWindow, INT const iMilliSecs )
    {
        m_ViewCol.AttachObserver( pRootWindow, iMilliSecs );
    }
    
    GridPoint       const GetGridPoint( ) const { return m_gp; }
    HIST_GENERATION const GetGenBirth( )  const { return m_genBirth; }
    HIST_GENERATION const GetGenDeath( )  const { return m_genDeath; }
    BOOL            const IsInGrid( )     const { return m_gp.IsInGrid( ); }
    BOOL            const IsAlive( )      const { return m_pModelWork->IsAlive( m_gp ); }
    BOOL            const IsDead( )       const { return m_pModelWork->IsDead ( m_gp ); }

private:
    EvoHistorySys * m_pEvoHistorySys;
    ModelData     * m_pModelWork;
    ViewCollection  m_ViewCol;
    GridPoint       m_gp;
    HIST_GENERATION m_genBirth;
    HIST_GENERATION m_genDeath;
};
