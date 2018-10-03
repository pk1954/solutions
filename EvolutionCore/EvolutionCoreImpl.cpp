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
#include "ObserverInterface.h"
#include "EvolutionCoreWrappers.h"
#include "EvolutionCoreImpl.h"

using namespace std;

EvolutionCoreImpl::EvolutionCoreImpl( ) :
    m_pObservers( nullptr ),
	m_brush( & m_grid )
{ 
	ResetAll( );
};

EvolutionCoreImpl::~EvolutionCoreImpl( )
{
    m_pObservers = nullptr;
}

void EvolutionCoreImpl::ResetAll( )
{
    m_grid.ResetGrid( );
	m_idPOI.ResetIndId( );
	m_plan.SetInvalid( );
	ResetSelection( );
	m_brush.Reset( );
	m_bSimulationMode = false;
}

void EvolutionCoreImpl::SetPoi( GridPoint const &  gp )       
{ 
	if ( gp.IsInGrid( ) )
	{
		IndId const idPoiNew = GetId( gp );
		if ( idPoiNew.IsDefined( ) )
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
			: GridPoint::GP_NULL; 
}

// Compute - plan and implement one generation step for all living individuals
//           let food grow 
//           increment generation number

void EvolutionCoreImpl::Compute( )
{
	PlannedActivity * pPlan = GetPlan4Writing( );

    GplIterator m_gplIterator( m_grid );

    GridPoint gpRun = m_gplIterator.Begin( );

    while ( gpRun.IsNotNull( ) )
    {
        assert( gpRun.IsInGrid( ) );
        assert( m_grid.IsAlive( gpRun ) );

        m_grid.MakePlan( gpRun, * pPlan );
        pPlan->SetValid( );
        if ( (m_pObservers != nullptr) && IsPoiDefined( ) ) 
        {
            if ( IsPoi( gpRun ) || IsPoi( pPlan->GetPartner( ) ) )
               m_pObservers->Notify( true );
        }
        pPlan->SetInvalid( );
        gpRun = m_grid.ImplementPlan( gpRun, * pPlan );   // may return GP_NULL
    }

    m_grid.FoodGrowth( );
    m_grid.IncGenNr( );
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
	