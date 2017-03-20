// EvolutionCoreImpl.cpp
//

#include "stdafx.h"
#include "assert.h"
#include <array>
#include "dump.h"
#include "wrappers.h"
#include "config.h"
#include "strategy.h"
#include "grid_model.h"
#include "gridCircle.h"
#include "gplIterator.h"
#include "DisplayFunctor.h"
#include "EvolutionCoreImpl.h"

using namespace std;

int EvolutionCoreImpl::m_iStdCapacity;

EvolutionCoreImpl::EvolutionCoreImpl( ) :
    m_plan( ),
    m_idPOI( ),
    m_gridDisplayFunctor( nullptr )
{ 
}

EvolutionCoreImpl::~EvolutionCoreImpl( )
{
    m_gridDisplayFunctor = nullptr;
}

void EvolutionCoreImpl::ResetModel( EvolutionModelData * const pModel )
{
    EvolutionModelDataImpl * pModelImpl = static_cast< EvolutionModelDataImpl * >( pModel );
    pModelImpl->m_grid.ResetGrid( );

    m_idPOI.ResetIndId( );
    m_plan.SetInvalid( );
    m_iStdCapacity = Config::GetConfigValueShort( Config::tId::stdCapacity );
}

// Compute - plan and implement one generation step for all living individuals
//           let food grow 
//           increment generation number

void EvolutionCoreImpl::Compute( EvolutionModelData * const pModel )
{
    EvolutionModelDataImpl * pModelImpl = static_cast< EvolutionModelDataImpl * >( pModel );

    GplIterator m_gplIterator( pModelImpl->m_grid );

    GridPoint gpRun = m_gplIterator.Begin( );

    while ( gpRun.IsNotNull( ) )
    {
        assert( gpRun.IsInGrid( ) );
        assert( pModelImpl->m_grid.IsAlive( gpRun ) );

        pModelImpl->m_grid.MakePlan( gpRun, m_plan );
        m_plan.SetValid( );
        if ( IsPoiDefined( ) ) 
        {
            if ( IsPoi( pModel, gpRun ) || IsPoi( pModel, m_plan.GetPartner( ) ) )
               ( void )( * m_gridDisplayFunctor )( true );
        }
        m_plan.SetInvalid( );
        gpRun = pModelImpl->m_grid.ImplementPlan( gpRun, m_plan );   // may return GP_NULL
    }

    pModelImpl->m_grid.FoodGrowth( );
    pModelImpl->m_grid.IncGenNr( );
}

void EvolutionCoreImpl::DumpGridPointList( ) const
{
    //    dumpGridPointList( m_histCacheItem.m_grid );    // TODO: Make configurable 
}

void EvolutionCoreImpl::dumpGridPointList( Grid const & grid ) const
{
    int iCount = 0;
    DUMP::Dump( L"#  +++ start dump of GridPoint list" );
    GplIterator iter( grid );
    for ( (void)iter.Begin( ); iter.IsNotAtEnd( ); (void)iter.GotoNext( ) )
    {
        DUMP::Dump( grid, iter.GetCurrent( ) );
        DUMP::Dump( L"" );
        if ( ++iCount >= 1000 )
        {
            DUMP::Dump( L"# +++ dumpGridPointList loop counter exceeded " );
            break;
        }
    }
    DUMP::Dump( L"#  +++ end of dump" );
}
