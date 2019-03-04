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

std::array< GeneTypeLimits, GeneType::COUNT      > Genome::m_aLimitsGeneral;
std::array< GeneTypeLimits,   Action::COUNT      > Genome::m_aLimitsActions;
std::array< bool,             Action::NR_ACTIONS > Genome::m_abActionEnabled;

std::array< unsigned int, Genome::MAX_LIFE_SPAN + 1 > Genome::m_mortalityTable;

void Genome::setGeneralLimits( GeneType::Id const gene, long const lLo, long const lHi )
{
    m_aLimitsGeneral[ static_cast<int>( gene ) ].SetLimits( lLo, lHi );
}

void Genome::InitClass( )
{ 
	double dRandomMax = static_cast<double>(Random::MAX_VAL);

	for ( int age = 0; age <= MAX_LIFE_SPAN; ++age )
	{
		double dAge = static_cast<double>(age);
		double dx   = dAge / MAX_LIFE_SPAN;
		double dx2  = dx * dx;
		double dx4  = dx2 * dx2;
		double dx8  = dx4 * dx4;
		double dAgeFactor = dx8 * dRandomMax;
		AssertLimits( dAgeFactor, 0.0, dRandomMax );
		m_mortalityTable[ age ] = static_cast<unsigned int>( dAgeFactor );
	}
    // Init limitations

    for ( auto & lim : m_aLimitsActions )
    {
        lim.SetLimits( 1, 1000 );
    }

    setGeneralLimits( GeneType::Id::appetite,           1, Config::GetConfigValue( Config::tId::maxFood      ) );
    setGeneralLimits( GeneType::Id::fertilInvest,       1, Config::GetConfigValue( Config::tId::maxFood      ) );
    setGeneralLimits( GeneType::Id::memSize,            1, IMEMSIZE_MAX );
    setGeneralLimits( GeneType::Id::thresholdClone,     0, Config::GetConfigValue( Config::tId::stdCapacity  ) );
    setGeneralLimits( GeneType::Id::thresholdMarry,     0, Config::GetConfigValue( Config::tId::stdCapacity  ) );
    setGeneralLimits( GeneType::Id::thresholdMove,      0, Config::GetConfigValue( Config::tId::stdCapacity  ) );
    setGeneralLimits( GeneType::Id::thresholdFertilize, 0, Config::GetConfigValue( Config::tId::stdCapacity  ) );
    setGeneralLimits( GeneType::Id::maxEat,             0, Config::GetConfigValue( Config::tId::stdCapacity  ) );
    setGeneralLimits( GeneType::Id::cloneDonation,      0, SHRT_MAX                                           );

    // init genome template 

    m_genomeTemplate.setActionGene( Action::Id::move,      500 );
    m_genomeTemplate.setActionGene( Action::Id::clone,     500 );
    m_genomeTemplate.setActionGene( Action::Id::marry,     500 );
    m_genomeTemplate.setActionGene( Action::Id::interact,  500 );
    m_genomeTemplate.setActionGene( Action::Id::eat,       500 );
    m_genomeTemplate.setActionGene( Action::Id::fertilize, 500 );

    m_genomeTemplate.setGeneralGene( GeneType::Id::appetite,           Config::GetConfigValue( Config::tId::defaultAppetite     ) );
    m_genomeTemplate.setGeneralGene( GeneType::Id::fertilInvest,       Config::GetConfigValue( Config::tId::defaultFertilInvest ) );
    m_genomeTemplate.setGeneralGene( GeneType::Id::memSize,            Config::GetConfigValue( Config::tId::stdMemSize          ) );
    m_genomeTemplate.setGeneralGene( GeneType::Id::thresholdClone,     Config::GetConfigValue( Config::tId::thresholdClone      ) );
    m_genomeTemplate.setGeneralGene( GeneType::Id::thresholdMarry,     Config::GetConfigValue( Config::tId::thresholdMarry      ) );
    m_genomeTemplate.setGeneralGene( GeneType::Id::thresholdMove,      Config::GetConfigValue( Config::tId::thresholdMove       ) );
    m_genomeTemplate.setGeneralGene( GeneType::Id::thresholdFertilize, Config::GetConfigValue( Config::tId::thresholdFertilize  ) );
    m_genomeTemplate.setGeneralGene( GeneType::Id::maxEat,             Config::GetConfigValue( Config::tId::maxEat              ) );
    m_genomeTemplate.setGeneralGene( GeneType::Id::cloneDonation,      SHRT_MAX / 2 );

    // static members for caching frequently used configuration items

    enabled( Action::Id::move      ) = Config::GetConfigValue( Config::tId::moveEnabled      ) > 0;
    enabled( Action::Id::fertilize ) = Config::GetConfigValue( Config::tId::fertilizeEnabled ) > 0;
    enabled( Action::Id::passOn    ) = Config::GetConfigValue( Config::tId::passOnEnabled    ) > 0;
    enabled( Action::Id::clone     ) = Config::GetConfigValue( Config::tId::cloneEnabled     ) > 0;
    enabled( Action::Id::marry     ) = Config::GetConfigValue( Config::tId::marryEnabled     ) > 0;
    enabled( Action::Id::interact  ) = Config::GetConfigValue( Config::tId::interactEnabled  ) > 0;
    enabled( Action::Id::eat       ) = Config::GetConfigValue( Config::tId::eatEnabled       ) > 0;
}

//  nonstatic functions 

Genome::Genome( )
{
    InitGenome( );
}

void Genome::setActionGene( Action::Id const action, int const iValue )
{
    short const sValue = static_cast<short>( iValue );
    m_aLimitsActions[ static_cast<int>( action ) ].CheckLimits( sValue );
    m_aGeneActions  [ static_cast<int>( action ) ].m_gene.SetAllele( sValue );
    m_aGeneActions  [ static_cast<int>( action ) ].m_action = action;
}

void Genome::setGeneralGene( GeneType::Id const type, int const iValue )
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

Action::Id Genome::GetOption
( 
    bool           const bHasFreeSpace, 
    bool           const bHasNeighbor,
    int            const iEnergy,
	EVO_GENERATION const age,
    Random             & random 
) const
{
	if (
		  ( m_abActionEnabled[ static_cast<int>( Action::Id::passOn ) ] ) && 
		  ( m_mortalityTable[ age.GetValue() ] > random.NextRandomNumber() )
	   )
		return Action::Id::passOn;
	
	std::array <bool, Action::COUNT > abOptions;
 
	abOptions[ static_cast<int>( Action::Id::move      ) ] = bHasFreeSpace &&                 ( iEnergy >= GetAllele( GeneType::Id::thresholdMove )      );
    abOptions[ static_cast<int>( Action::Id::fertilize ) ] =                                  ( iEnergy >= GetAllele( GeneType::Id::thresholdFertilize ) );
    abOptions[ static_cast<int>( Action::Id::clone     ) ] = bHasFreeSpace &&                 ( iEnergy >= GetAllele( GeneType::Id::thresholdClone )     );
    abOptions[ static_cast<int>( Action::Id::marry     ) ] = bHasFreeSpace && bHasNeighbor && ( iEnergy >= GetAllele( GeneType::Id::thresholdMarry )     );
    abOptions[ static_cast<int>( Action::Id::interact  ) ] =                  bHasNeighbor;
    abOptions[ static_cast<int>( Action::Id::eat       ) ] =                                  ( iEnergy <  GetAllele( GeneType::Id::maxEat )             );

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

	return Action::Id::undefined;
}
