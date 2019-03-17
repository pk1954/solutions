// EvolutionCoreImpl.cpp
//

#include "stdafx.h"
#include "assert.h"
#include <array>
#include "dump.h"
#include "config.h"
#include "strategy.h"
#include "grid_model.h"
#include "gplIterator.h"
#include "EventInterface.h"
#include "ObserverInterface.h"
#include "EvolutionCoreWrappers.h"
#include "EvolutionCoreImpl.h"

ObserverInterface * EvolutionCoreImpl::m_pObservers = nullptr;    // GUI call back for display of current model 
EventInterface    * EvolutionCoreImpl::m_pEventPOI  = nullptr;

EvolutionCoreImpl::EvolutionCoreImpl( ) :
	m_brush( & m_grid )
{ 
	ResetAll( );
};

EvolutionCoreImpl::~EvolutionCoreImpl( ) { }

void EvolutionCoreImpl::ResetAll( )
{
    m_grid.ResetGrid( );
	m_idPOI.Set2Null( );
	m_plan.SetInvalid( );
	m_brush.Reset( );
	m_bSimulationMode = false;
}

void EvolutionCoreImpl::SetPoi( GridPoint const  gp )       
{ 
	if ( IsInGrid( gp ) )
	{
		IND_ID const idPoiNew = GetId( gp );
		if ( idPoiNew.IsNotNull( ) )
		{    
			if ( IsPoiId( idPoiNew ) )
				ClearPoi( );           // same POI. deactivate POI
			else
				m_idPOI = m_grid.GetId( gp ); 
		}
	}
}

GridPoint EvolutionCoreImpl::FindPOI( ) const 
{ 
	return IsPoiDefined( ) 
			? FindGridPoint( m_idPOI ) 
			: GridPoint::NULL_VAL(); 
}

GridPoint EvolutionCoreImpl::FindGridPoint( IND_ID const & id ) const 
{ 
	return ( id.IsNull() )
			? GridPoint::NULL_VAL()
			: m_grid.FindGridPoint
			  ( 
				  [&](GridPoint const gp) { return (GetId(gp) == id); }, 
				  GridDimensions::GridRectFull() 
		      );
}

// Compute - plan and implement one generation step for all living individuals
//           let food grow 
//           increment generation number

void EvolutionCoreImpl::Compute( )
{
    GplIterator       m_gplIterator( m_grid );
    GridPoint         gpRun = m_gplIterator.Begin( );
	PlannedActivity * pPlan = GetPlan4Writing( );

	m_grid.PrepareActionCounters( );
    while ( gpRun.IsNotNull( ) )
    {
        assert( IsInGrid( gpRun ) );
        assert( m_grid.IsAlive( gpRun ) );

        m_grid.MakePlan( gpRun, * pPlan );
		stopOnPoi      ( gpRun, * pPlan );
        gpRun = m_grid.ImplementPlan( gpRun, * pPlan );   // may return NULL_VAL
    }

    m_grid.FoodGrowth( );
    m_grid.IncGenNr( );
}

void EvolutionCoreImpl::stopOnPoi
( 
    GridPoint       const gpRun, 
    PlannedActivity     & plan
)
{
    if ( (m_pObservers != nullptr) && IsPoiDefined( ) ) 
    {
        if ( IsPoi( gpRun ) || IsPoi( plan.GetPartner( ) ) )
		{
		    plan.SetValid( );
            m_pObservers->Notify( true );  // stop immediately
            m_pEventPOI->Wait( );
		    plan.SetInvalid( );
		}
    }
}

void EvolutionCoreImpl::DumpGridPointList( ) const
{
    int iCount = 0;
    DUMP::Dump( L"#  +++ start dump of GridPoint list" );
	DUMP::DumpNL( );
    GplIterator iter( m_grid );
    for ( (void)iter.Begin( ); iter.IsNotAtEnd( ); (void)iter.GotoNext( ) )
    {
		GridField gf = m_grid.GetGridField( iter.GetCurrent( ) );
        DUMP::Dump( m_grid, iter.GetCurrent( ) );
        DUMP::Dump( L"" );
        if ( ++iCount >= 1000 )
        {
            DUMP::Dump( L"# +++ dumpGridPointList loop counter exceeded " );
            break;
        }
    }
    DUMP::Dump( L"#  +++ end of dump" );
	DUMP::DumpNL( );
}
	