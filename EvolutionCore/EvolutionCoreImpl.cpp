// EvolutionCoreImpl.cpp
//

#include "stdafx.h"
#include "assert.h"
#include <array>
#include "dump.h"
#include "EvolutionCoreWrappers.h"
#include "config.h"
#include "strategy.h"
#include "grid_model.h"
#include "gplIterator.h"
#include "DisplayFunctor.h"
#include "EvolutionModelDataImpl.h"
#include "EvolutionCoreImpl.h"

using namespace std;

EvolutionCoreImpl::EvolutionCoreImpl( ) :
    m_gridDisplayFunctor( nullptr )
{ }

EvolutionCoreImpl::~EvolutionCoreImpl( )
{
    m_gridDisplayFunctor = nullptr;
}

// Compute - plan and implement one generation step for all living individuals
//           let food grow 
//           increment generation number

void EvolutionCoreImpl::Compute( EvolutionModelData * const pModel )
{
    EvolutionModelDataImpl * pModelImpl = static_cast< EvolutionModelDataImpl * >( pModel );
	PlannedActivity        * pPlan      = pModelImpl->GetPlan4Writing( );

    GplIterator m_gplIterator( pModelImpl->m_grid );

    GridPoint gpRun = m_gplIterator.Begin( );

    while ( gpRun.IsNotNull( ) )
    {
        assert( gpRun.IsInGrid( ) );
        assert( pModelImpl->m_grid.IsAlive( gpRun ) );

        pModelImpl->m_grid.MakePlan( gpRun, * pPlan );
        pPlan->SetValid( );
        if ( (m_gridDisplayFunctor != nullptr) && pModelImpl->IsPoiDefined( ) ) 
        {
            if ( pModelImpl->IsPoi( gpRun ) || pModelImpl->IsPoi( pPlan->GetPartner( ) ) )
               ( * m_gridDisplayFunctor )( true );
        }
        pPlan->SetInvalid( );
        gpRun = pModelImpl->m_grid.ImplementPlan( gpRun, * pPlan );   // may return GP_NULL
    }

    pModelImpl->m_grid.FoodGrowth( );
    pModelImpl->m_grid.IncGenNr( );
}

void EvolutionCoreImpl::DumpGridPointList( EvolutionModelData * const pModel ) const
{
	Grid const & grid = static_cast< EvolutionModelDataImpl * >( pModel )->m_grid;
    int iCount = 0;
    DUMP::Dump( L"#  +++ start dump of GridPoint list" );
	DUMP::DumpNL( );
    GplIterator iter( grid );
    for ( (void)iter.Begin( ); iter.IsNotAtEnd( ); (void)iter.GotoNext( ) )
    {
		GridField gf = grid.GetGridField( iter.GetCurrent( ) );
        DUMP::Dump( grid, iter.GetCurrent( ) );
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
	