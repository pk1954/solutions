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
#include "EvolutionCoreWrappers.h"
#include "EvolutionCoreImpl.h"

EvolutionCoreImpl::EvolutionCoreImpl( ) :
	m_brush( & m_grid ),
	m_bSimulationMode( false )
{ 
	ResetAll( );
};

EvolutionCoreImpl::~EvolutionCoreImpl( ) { }

void EvolutionCoreImpl::ResetAll( )
{
    m_grid.ResetGrid( );
	m_brush.Reset( );
	m_bSimulationMode = false;
	GridPOI::ClearPoi( );
}

// Compute - plan and implement one generation step for all living individuals
//           let food grow 
//           increment generation number

void EvolutionCoreImpl::Compute( )
{
    GplIterator m_gplIterator( m_grid );
    GridPoint   gpRun = m_gplIterator.Begin( );

	m_grid.PrepareComputation( );
    while ( gpRun.IsNotNull( ) )
    {
        assert( IsInGrid( gpRun ) );
        assert( m_grid.IsAlive( gpRun ) );

		gpRun = m_grid.ComputeNextGeneration( gpRun );   // may return NULL_VAL  
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
	