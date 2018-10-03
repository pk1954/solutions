// win32_focusPoint.cpp
//

#include "stdafx.h"
#include "EvoHistorySysGlue.h"
#include "EvolutionCore.h"
#include "win32_viewCollection.h"
#include "win32_focusPoint.h"

FocusPoint::FocusPoint( ) :
    m_pEvoHistGlue( nullptr ),
    m_pCore( nullptr ),
    m_genBirth( - 1 ),
    m_genDeath( - 1 ),
    m_gp( )
{ };

void FocusPoint::Start
( 
    EvoHistorySysGlue * pEvoHistGlue,
    EvolutionCore     * pCore
)
{
    assert( pCore != nullptr );
    m_pEvoHistGlue = pEvoHistGlue;
    m_pCore        = pCore;
}

void FocusPoint::SetFocusPoint( GridPoint const gpNew )
{
    if ( gpNew != m_gp )
    {
        m_gp = gpNew;
        if ( IsInGrid( ) )
        {
            if ( m_pEvoHistGlue != nullptr )
            {
                IndId id = m_pCore->GetId( m_gp );
                m_genBirth = m_pEvoHistGlue->GetFirstGenOfIndividual( id );
                m_genDeath = m_pEvoHistGlue->GetLastGenOfIndividual ( id ) + 1;
            }
        }
        m_ViewCollection.Notify( );
    }
}
