// EvoStatistics.cpp :
//
// EvolutionCore

#include "stdafx.h"
#include "TextBuffer.h"
#include "EvolutionCore.h"
#include "EvoStatistics.h"

EvoStatistics::EvoStatistics( ) :
	m_pTextBuf( nullptr )
{ }

EvoStatistics::~EvoStatistics()
{
	m_pTextBuf = nullptr;
}

void EvoStatistics::Prepare
( 
    EvolutionCore const & core,
	GridRect      const gridRectSelection,
	TextBuffer  * const pTextBuf 
)
{
	m_pTextBuf = pTextBuf;

	m_gsCounter.zero();         
    m_gsAverageAge.zero();      
	m_XaAction  .Apply2All( [&](auto & elem) { elem = 0; } );
	m_XaGenes   .Apply2All( [&](auto & elem) { elem = 0; } );
	m_auiMemSize.Apply2All( [&](auto & elem) { elem = 0; } );

	Apply2Rect
	( 
		[&](GridPoint const gp)
		{
			if ( core.IsAlive( gp ) )
				aquireData( core, gp );
			return false;
		},
		gridRectSelection
	);

	scaleData( );
};

void EvoStatistics::aquireData
(
	EvolutionCore const & core, 
	GridPoint     const & gp 
)
{
	EVO_GENERATION const evoGen   { core.GetEvoGenerationNr() };
	Strategy::Id   const strategy { core.GetStrategyId( gp ) };
	EVO_GENERATION const age      { core.GetAge( gp ) };
	MEM_INDEX      const memSize  { core.GetMemSize( gp ) };

	assert( strategy != Strategy::Id::empty );
	assert( age.IsNotNull() );
	
	GeneType::Apply2AllEnabledGeneTypes
	(
		[&]( auto geneType )
		{
			m_XaGenes[geneType].Add( strategy, CastToUnsignedInt( core.GetAllele( gp, geneType ) ) );
		}
	);

	Action::Apply2AllEnabledActions
	(
		[&]( auto action )
		{
			GeneType::Id geneType = GetRelatedGeneType(action);
			if ( GeneType::IsDefined( geneType ) ) 
				m_XaAction[action].Add( strategy, static_cast<float>( core.GetAllele( gp, geneType ) ) );
		}
	);

	m_auiMemSize  [strategy] += memSize.GetValue();
	m_gsAverageAge[strategy] += age.GetValue();
	m_gsAverageAge.Add( strategy, age.GetValue() );
	m_gsCounter.Add( strategy, 1 );
}

void EvoStatistics::scale( float & op, float const div )
{
    if ( div == 0 )
        op = 0;
    else
	{
		assert( op <= (std::numeric_limits<float>::max)() / 100);
        op = (op * 100 + 50 ) / div;
	}
}

void EvoStatistics::scaleData( )
{
	m_gsAverageAge.DivNonZero( m_gsCounter );

	GeneType::Apply2AllEnabledGeneTypes
	(
		[&]( auto geneType )
		{
			m_XaGenes[geneType].DivNonZero( m_gsCounter );
		}
	);

	float fSum { 0.0 };
	m_XaAction.Apply2All( [&](XaCounter<float> & elem) { fSum += elem.General(); } );
	m_XaAction.Apply2All( [&](XaCounter<float> & elem) { scale( elem.General(), fSum ); } );
	
	Strategy::Apply2All
	( 
		[&]( Strategy::Id strategy )
		{
			float fSum { 0.0 };
			m_XaAction.Apply2All( [&](XaCounter<float> & elem) { fSum += elem[strategy]; } );
			m_XaAction.Apply2All( [&](XaCounter<float> & elem) { scale(  elem[strategy], fSum ); } );
		}
	);

	Strategy::Apply2All
	( 
		[&]( Strategy::Id strategy )
		{ DivNonZero( m_auiMemSize[strategy], m_gsCounter[strategy] ); }
	);
};

void EvoStatistics::printHeader( )
{
	Strategy::Apply2All
	( 
		[&]( Strategy::Id const & id ) 
		{ m_pTextBuf->printString( Strategy::GetName( id ) ); }
	);
	m_pTextBuf->printString( L"all" );                               
}

void EvoStatistics::printCounter( wchar_t const * const data )
{
    m_gsCounter.printGeneLine( m_pTextBuf, data );
}

void EvoStatistics::printAvAge( wchar_t const * const data )
{
    m_gsAverageAge.printGeneLine( m_pTextBuf, data );
}

void EvoStatistics::printCounters
(
	EvolutionCore const & core,
	Action::Id    const   action 
)
{
	ACTION_COUNT sum { 0 };
	m_pTextBuf->nextLine( Action::GetName( action ) );
	Strategy::Apply2All
	( 
		[&]( Strategy::Id strat )
		{
			ACTION_COUNT counter { core.GetActionCounter( strat, action ) };
			m_pTextBuf->printNumber( counter.GetValue() );
			sum += counter;
		}
	);
	m_pTextBuf->printNumber( sum.GetValue() );
}

void EvoStatistics::printIncidence( EvolutionCore const & core )
{
	Action::Apply2AllEnabledActions( [&]( Action::Id action ) {	printCounters( core, action ); } );
}

void EvoStatistics::printProbabilities( )
{
	Action::Apply2AllEnabledActions
	(
		[&]( Action::Id action )
		{
			if ( GeneType::IsDefined( GetRelatedGeneType( action ) )  )
				m_XaAction[action].printGeneLine( m_pTextBuf, Action::GetName( action ) );
		}
	);

}

void EvoStatistics::printGeneStat( )
{
	GeneType::Apply2AllEnabledGeneTypes
	(
		[&]( GeneType::Id geneType )
		{
			m_XaGenes[geneType].printGeneLine( m_pTextBuf, GeneType::GetName( geneType ) );
		}
	);
}

void EvoStatistics::printAvFood( wchar_t const * const data )
{
    XaCounter<float> fsAvFood;

	Strategy::Apply2All
	( 
		[&]( Strategy::Id strategy )
		{ 
			fsAvFood[strategy] = m_XaGenes[GeneType::Id::appetite][strategy] * m_XaAction[Action::Id::eat][strategy] / 100; 
		}
	);

    fsAvFood.General() = m_XaGenes[GeneType::Id::appetite].General() * m_XaAction[Action::Id::eat].General() / 100 ;

    fsAvFood.printGeneLine( m_pTextBuf, data );
}

void EvoStatistics::printMemory( wchar_t const * const data )
{
    m_pTextBuf->nextLine( data );

	Strategy::Apply2All([&](Strategy::Id strategy) { m_pTextBuf->printNumber(m_auiMemSize[strategy]); });
}
