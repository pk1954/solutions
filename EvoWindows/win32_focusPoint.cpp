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

// Feature deactiveted, takes too much CPU time, stalls UI
// Options: 
//    - Run in separate thread
//    - Optimize (??)
//    - Call only on request
//
//        if ( IsInGrid( ) )
//        {
//            if ( m_pEvoHistGlue != nullptr )
//            {
//                IndividualId id = m_pCore->GetId( m_gp );
//                m_genBirth = m_pEvoHistGlue->GetFirstGenOfIndividual( id );
//                m_genDeath = m_pEvoHistGlue->GetLastGenOfIndividual ( id ) + 1;
//            }
//        }

        m_ViewCollection.Notify( false );
    }
}
