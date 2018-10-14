// genome.cpp : 
//

#include "stdafx.h"
#include <array>
#include "assert.h"
#include "debug.h"
#include "config.h"
#include "strategy.h"
#include "genome.h"
#include "random.h"

//lint  -e1566    initialization 

// static members and functions

Genome Genome::m_genomeTemplate;

array< GeneTypeLimits, NR_GENES        > Genome::m_aLimitsGeneral;
array< GeneTypeLimits, NR_ACTION_GENES > Genome::m_aLimitsActions;
array< bool,           NR_ACTIONS      > Genome::m_abActionEnabled;

array< unsigned int, Genome::MAX_LIFE_SPAN + 1 > Genome::m_mortalityTable;

void Genome::setGeneralLimits( tGeneType const gene, long const lLo, long const lHi )
{
    m_aLimitsGeneral[ static_cast<int>( gene ) ].SetLimits( lLo, lHi );
}

void Genome::InitClass( )
{ 
	double dRandomMax = static_cast<double>(Random::MAX_VAL);

	for ( EVO_GENERATION age = 0; age <= MAX_LIFE_SPAN; ++age )
	{
		double dAge = static_cast<double>(age);
		double dx   = dAge / MAX_LIFE_SPAN;
		double dx2  = dx * dx;
		double dx4  = dx2 * dx2;
		double dx8  = dx4 * dx4;
		double dAgeFactor = dx8 * dRandomMax;
		ASSERT_LIMITS( dAgeFactor, 0.0, dRandomMax );
		m_mortalityTable[ age ] = static_cast<unsigned int>( dAgeFactor );
	}
    // Init limitations

    for ( auto & lim : m_aLimitsActions )
    {
        lim.SetLimits( 1, 1000 );
    }

    setGeneralLimits( tGeneType::appetite,           1, Config::GetConfigValue( Config::tId::maxFood      ) );
    setGeneralLimits( tGeneType::fertilInvest,       1, Config::GetConfigValue( Config::tId::maxFood      ) );
    setGeneralLimits( tGeneType::memSize,            1, StrategyData::IMEMSIZE_MAX                         );
    setGeneralLimits( tGeneType::thresholdClone,     0, Config::GetConfigValue( Config::tId::stdCapacity  ) );
    setGeneralLimits( tGeneType::thresholdMarry,     0, Config::GetConfigValue( Config::tId::stdCapacity  ) );
    setGeneralLimits( tGeneType::thresholdMove,      0, Config::GetConfigValue( Config::tId::stdCapacity  ) );
    setGeneralLimits( tGeneType::thresholdFertilize, 0, Config::GetConfigValue( Config::tId::stdCapacity  ) );
    setGeneralLimits( tGeneType::maxEat,             0, Config::GetConfigValue( Config::tId::stdCapacity  ) );
    setGeneralLimits( tGeneType::cloneDonation,      0, SHRT_MAX                                           );

    // init genome template 

    m_genomeTemplate.setActionGene( tAction::move,      500 );
    m_genomeTemplate.setActionGene( tAction::clone,     500 );
    m_genomeTemplate.setActionGene( tAction::marry,     500 );
    m_genomeTemplate.setActionGene( tAction::interact,  500 );
    m_genomeTemplate.setActionGene( tAction::eat,       500 );
    m_genomeTemplate.setActionGene( tAction::fertilize, 500 );

    m_genomeTemplate.setGeneralGene( tGeneType::appetite,           Config::GetConfigValue( Config::tId::defaultAppetite     ) );
    m_genomeTemplate.setGeneralGene( tGeneType::fertilInvest,       Config::GetConfigValue( Config::tId::defaultFertilInvest ) );
    m_genomeTemplate.setGeneralGene( tGeneType::memSize,            Config::GetConfigValue( Config::tId::stdMemSize          ) );
    m_genomeTemplate.setGeneralGene( tGeneType::thresholdClone,     Config::GetConfigValue( Config::tId::thresholdClone      ) );
    m_genomeTemplate.setGeneralGene( tGeneType::thresholdMarry,     Config::GetConfigValue( Config::tId::thresholdMarry      ) );
    m_genomeTemplate.setGeneralGene( tGeneType::thresholdMove,      Config::GetConfigValue( Config::tId::thresholdMove       ) );
    m_genomeTemplate.setGeneralGene( tGeneType::thresholdFertilize, Config::GetConfigValue( Config::tId::thresholdFertilize  ) );
    m_genomeTemplate.setGeneralGene( tGeneType::maxEat,             Config::GetConfigValue( Config::tId::maxEat              ) );
    m_genomeTemplate.setGeneralGene( tGeneType::cloneDonation,      SHRT_MAX / 2 );

    // static members for caching frequently used configuration items

    enabled( tAction::move      ) = Config::GetConfigValue( Config::tId::moveEnabled      ) > 0;
    enabled( tAction::fertilize ) = Config::GetConfigValue( Config::tId::fertilizeEnabled ) > 0;
    enabled( tAction::passOn    ) = Config::GetConfigValue( Config::tId::passOnEnabled    ) > 0;
    enabled( tAction::clone     ) = Config::GetConfigValue( Config::tId::cloneEnabled     ) > 0;
    enabled( tAction::marry     ) = Config::GetConfigValue( Config::tId::marryEnabled     ) > 0;
    enabled( tAction::interact  ) = Config::GetConfigValue( Config::tId::interactEnabled  ) > 0;
    enabled( tAction::eat       ) = Config::GetConfigValue( Config::tId::eatEnabled       ) > 0;
}

//  nonstatic functions 

Genome::Genome( )
{
    InitGenome( );
}

void Genome::setActionGene( tAction const action, int const iValue )
{
    short const sValue = static_cast<short>( iValue );
    m_aLimitsActions[ static_cast<int>( action ) ].CheckLimits( sValue );
    m_aGeneActions  [ static_cast<int>( action ) ].m_gene.SetAllele( sValue );
    m_aGeneActions  [ static_cast<int>( action ) ].m_action = action;
}

void Genome::setGeneralGene( tGeneType const type, int const iValue )
{
    short const sValue = static_cast<short>( iValue );
    m_aLimitsGeneral[ static_cast<int>( type ) ].CheckLimits( sValue );
    m_aGeneGeneral  [ static_cast<int>( type ) ].m_gene.SetAllele( sValue );
    m_aGeneGeneral  [ static_cast<int>( type ) ].m_type = type;
}

void Genome::InitGenome( )
{
    if ( this != & m_genomeTemplate )
        * this = m_genomeTemplate;
}

void Genome::Mutate( short const sMutationRate, Random & random )
{
	double dMutationRate = static_cast<double>(sMutationRate);

    for ( auto & g : m_aGeneGeneral )
    {
        g.m_gene.Mutate( dMutationRate, m_aLimitsGeneral[ static_cast<int>( g.m_type ) ], random );
    }                                                                                                                     // verified in assembly code  

    for ( auto & g : m_aGeneActions )
    {
        g.m_gene.Mutate( dMutationRate, m_aLimitsActions[ static_cast<int>( g.m_action ) ], random );
    }
}

void Genome::Recombine( Genome const & genomeA, Genome const & genomeB, Random & random )
{
    for ( auto & g : m_aGeneGeneral )
    {
        Genome genome     = random.NextBooleanValue( ) ? genomeA : genomeB;
        short  sNewAllele = genome.m_aGeneGeneral[ static_cast<int>( g.m_type ) ].m_gene.GetAllele( );
        g.m_gene.SetAllele( sNewAllele );
    }                                                                                                                     // verified in assembly code  

    for ( auto & g : m_aGeneActions )
    {
        Genome genome     = random.NextBooleanValue( ) ? genomeA : genomeB;
        short  sNewAllele = genome.m_aGeneActions[ static_cast<int>( g.m_action ) ].m_gene.GetAllele( );
        g.m_gene.SetAllele( sNewAllele );
    }
}

tAction Genome::GetOption
( 
    bool           const bHasFreeSpace, 
    bool           const bHasNeighbor,
    int            const iEnergy,
	EVO_GENERATION const age,
    Random             & random 
) const
{
	if (
		  ( m_abActionEnabled[ static_cast<int>( tAction::passOn ) ] ) && 
		  ( m_mortalityTable[ age ] > random.NextRandomNumber() )
	   )
		return tAction::passOn;
	
	array <bool, NR_ACTION_GENES > abOptions;
 
	abOptions[ static_cast<int>( tAction::move      ) ] = bHasFreeSpace &&                 ( iEnergy >= GetAllele( tGeneType::thresholdMove )      );
    abOptions[ static_cast<int>( tAction::fertilize ) ] =                                  ( iEnergy >= GetAllele( tGeneType::thresholdFertilize ) );
    abOptions[ static_cast<int>( tAction::clone     ) ] = bHasFreeSpace &&                 ( iEnergy >= GetAllele( tGeneType::thresholdClone )     );
    abOptions[ static_cast<int>( tAction::marry     ) ] = bHasFreeSpace && bHasNeighbor && ( iEnergy >= GetAllele( tGeneType::thresholdMarry )     );
    abOptions[ static_cast<int>( tAction::interact  ) ] =                  bHasNeighbor;
    abOptions[ static_cast<int>( tAction::eat       ) ] =                                  ( iEnergy <  GetAllele( tGeneType::maxEat )             );

    unsigned int uiSum = 0;

    for ( auto & g : m_aGeneActions )
    {
        int index = static_cast<int>( g.m_action );
        if ( abOptions[ index ] && m_abActionEnabled[ index ] )
        {
            short const sVal = g.m_gene.GetAllele( );
            assert( sVal >= 0 );
            //lint -e571  suspicious cast
            unsigned int const uiVal = static_cast<unsigned int>( sVal );
            //lint +e571
            uiSum += uiVal;
        }
    }

    int iVal = random.NextRandomNumberScaledTo( uiSum );

    for ( auto & g : m_aGeneActions )
    {
        int index = static_cast<int>( g.m_action );
        if ( abOptions[ index ] )
            iVal -= g.m_gene.GetAllele( );
        if ( iVal < 0 )
        {
            assert( abOptions[ index ] );
            return g.m_action;
        }
    }

	return tAction::undefined;
}
