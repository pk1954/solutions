// win32_focusPoint.cpp
//

#include "stdafx.h"
#include "EvoHistorySys.h"
#include "EvolutionModelData.h"
#include "win32_focusPoint.h"

void FocusPoint::Start
( 
    EvoHistorySys      * pEvoHistorySys,
    EvolutionModelData * pModel
)
{
    assert( pModel != nullptr );
    m_pEvoHistorySys = pEvoHistorySys;
    m_pModelWork = pModel;
}

void FocusPoint::SetGridPoint( GridPoint const gpNew )
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

void FocusPoint::AttachFocusPointObserver( RootWindow const * pRootWindow, INT const iMilliSecs )
{
    m_ViewCol.AttachObserver( pRootWindow, iMilliSecs );
}
