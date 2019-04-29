// win32_focusPoint.cpp
//

#include "stdafx.h"
#include <iomanip>
#include "EvoHistorySysGlue.h"
#include "EvolutionCore.h"
#include "plannedActivity.h"
#include "win32_viewCollection.h"
#include "win32_focusPoint.h"

using std::endl;
using std::setw;
using std::wcout;
using std::setbase;

FocusPoint::FocusPoint( ) :
    m_pEvoHistGlue( nullptr ),
    m_pCore( nullptr ),
    //m_histGenBirth( ),
    //m_histGenDeath( ),
    m_gp( GridPoint::NULL_VAL() )
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

// Feature deactivated, takes too much CPU time, stalls UI
// Options: 
//    - Run in separate thread
//    - Optimize (??)
//    - Call only on request
//
//        if ( IsInGrid( ) )
//        {
//            if ( m_pEvoHistGlue != nullptr )
//            {
//                IND_ID id = m_pCore->GetId( m_gp );
//                m_histGenBirth = m_pEvoHistGlue->GetFirstGenOfIndividual( id );
//                m_histGenDeath = m_pEvoHistGlue->GetLastGenOfIndividual ( id ) + 1;
//            }
//        }

        m_ViewCollection.Notify( false );
    }
}

void FocusPoint::ShowIndividualHistory( )
{
	if ( IsInGrid( ) )
	{
	    if ( m_pEvoHistGlue != nullptr )
	    {
	        IND_ID          const id      = m_pCore->GetId( m_gp );
			HIST_GENERATION const genCurr = m_pEvoHistGlue->GetCurrentGeneration( );
			wcout << L" ID: " << setbase( 16 ) << setw(6) << id << setbase( 10 ) << endl;
			for ( HIST_GENERATION gen = genCurr; gen >= 0; --gen )
			{
				EvolutionCore const * pCore { m_pEvoHistGlue->GetEvolutionCore( gen ) };
				if ( pCore == nullptr )
					break;             // history cannot provide data for this generation

				GridPoint const gp { pCore->FindGridPoint( id ) };
				if ( gp.IsNull() )
					break;             // individual not yet born

				PlannedActivity plan { pCore->GetPlan( gp ) };
				if ( plan.GetActor().IsZero() )
					break;

				wcout << L" gen= "       << setw( 5) << gen;
				wcout << L" age= "       << setw( 3) << pCore->GetAge( m_gp );
				wcout << L" position= "  <<             m_gp;
				wcout << L" base cons= " << setw( 3) << plan.GetBaseConsumption( );
				wcout << L" action= "    << setw(10) << Action::GetName(plan.GetActionType());
				if ( plan.HasPartner() )
					wcout << L" partner= " << plan.GetPartner();
				else
					wcout << setw(21) << L" ";
				wcout << L" energy= "    << setw( 5) << pCore->GetEnergy( m_gp );
				wcout << endl;
			}
		}
	}
}
