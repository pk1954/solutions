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

std::array< bool, Action::COUNT > Genome::m_abActionEnabled; 

std::array< GeneTypeLimits, GeneralGeneType::COUNT > Genome::m_aLimitsGeneral;
std::array< GeneTypeLimits, ActionGeneType ::COUNT > Genome::m_aLimitsActions;

std::array< unsigned int, Genome::MAX_LIFE_SPAN + 1 > Genome::m_mortalityTable;

void Genome::setGeneralLimits( GeneralGeneType::Id const gene, long const lLo, long const lHi )
{
    m_aLimitsGeneral.at( static_cast<int>( gene ) ).SetLimits( lLo, lHi );
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

    setGeneralLimits( GeneralGeneType::Id::appetite,           1, Config::GetConfigValue( Config::tId::maxFood      ) );
    setGeneralLimits( GeneralGeneType::Id::fertilInvest,       1, Config::GetConfigValue( Config::tId::maxFood      ) );
    setGeneralLimits( GeneralGeneType::Id::memSize,            1, IMEMSIZE_MAX );
    setGeneralLimits( GeneralGeneType::Id::thresholdClone,     0, Config::GetConfigValue( Config::tId::stdCapacity  ) );
    setGeneralLimits( GeneralGeneType::Id::thresholdMarry,     0, Config::GetConfigValue( Config::tId::stdCapacity  ) );
    setGeneralLimits( GeneralGeneType::Id::thresholdMove,      0, Config::GetConfigValue( Config::tId::stdCapacity  ) );
    setGeneralLimits( GeneralGeneType::Id::thresholdFertilize, 0, Config::GetConfigValue( Config::tId::stdCapacity  ) );
    setGeneralLimits( GeneralGeneType::Id::maxEat,             0, Config::GetConfigValue( Config::tId::stdCapacity  ) );
    setGeneralLimits( GeneralGeneType::Id::cloneDonation,      0, SHRT_MAX                                           );

    // init genome template 

    m_genomeTemplate.setActionGene( ActionGeneType::Id::move,      500 );
    m_genomeTemplate.setActionGene( ActionGeneType::Id::clone,     500 );
    m_genomeTemplate.setActionGene( ActionGeneType::Id::marry,     500 );
    m_genomeTemplate.setActionGene( ActionGeneType::Id::interact,  500 );
    m_genomeTemplate.setActionGene( ActionGeneType::Id::eat,       500 );
    m_genomeTemplate.setActionGene( ActionGeneType::Id::fertilize, 500 );

    m_genomeTemplate.setGeneralGene( GeneralGeneType::Id::appetite,           Config::GetConfigValueShort( Config::tId::defaultAppetite     ) );
    m_genomeTemplate.setGeneralGene( GeneralGeneType::Id::fertilInvest,       Config::GetConfigValueShort( Config::tId::defaultFertilInvest ) );
    m_genomeTemplate.setGeneralGene( GeneralGeneType::Id::memSize,            Config::GetConfigValueShort( Config::tId::stdMemSize          ) );
    m_genomeTemplate.setGeneralGene( GeneralGeneType::Id::thresholdClone,     Config::GetConfigValueShort( Config::tId::thresholdClone      ) );
    m_genomeTemplate.setGeneralGene( GeneralGeneType::Id::thresholdMarry,     Config::GetConfigValueShort( Config::tId::thresholdMarry      ) );
    m_genomeTemplate.setGeneralGene( GeneralGeneType::Id::thresholdMove,      Config::GetConfigValueShort( Config::tId::thresholdMove       ) );
    m_genomeTemplate.setGeneralGene( GeneralGeneType::Id::thresholdFertilize, Config::GetConfigValueShort( Config::tId::thresholdFertilize  ) );
    m_genomeTemplate.setGeneralGene( GeneralGeneType::Id::maxEat,             Config::GetConfigValueShort( Config::tId::maxEat              ) );
    m_genomeTemplate.setGeneralGene( GeneralGeneType::Id::cloneDonation,      SHRT_MAX / 2 );

    // static members for caching frequently used configuration items

    enabled( Action::Id::move      ) = Config::GetConfigValueBool( Config::tId::moveEnabled      );
    enabled( Action::Id::fertilize ) = Config::GetConfigValueBool( Config::tId::fertilizeEnabled );
    enabled( Action::Id::passOn    ) = Config::GetConfigValueBool( Config::tId::passOnEnabled    );
    enabled( Action::Id::clone     ) = Config::GetConfigValueBool( Config::tId::cloneEnabled     );
    enabled( Action::Id::marry     ) = Config::GetConfigValueBool( Config::tId::marryEnabled     );
    enabled( Action::Id::interact  ) = Config::GetConfigValueBool( Config::tId::interactEnabled  );
    enabled( Action::Id::eat       ) = Config::GetConfigValueBool( Config::tId::eatEnabled       );
}

//  nonstatic functions 

Genome::Genome( )
{
    InitGenome( );
}

void Genome::setActionGene( ActionGeneType::Id const type, short const sValue )
{
    int const index = static_cast<int>( type );
	m_aLimitsActions.at( index ).CheckLimits( sValue );
    m_aGeneActions  .at( index ).m_gene.SetAllele( sValue );
    m_aGeneActions  .at( index ).m_type = type;
}

void Genome::setGeneralGene( GeneralGeneType::Id const type, short const sValue )
{
	int const index = static_cast<int>( type );
    m_aLimitsGeneral.at( index ).CheckLimits( sValue );
    m_aGeneGeneral  .at( index ).m_gene.SetAllele( sValue );
    m_aGeneGeneral  .at( index ).m_type = type;
}

void Genome::InitGenome( )
{
    if ( this != & m_genomeTemplate )
        * this = m_genomeTemplate;
}

void Genome::Mutate( PERCENT const mutationRate, Random & random )
{
	double dMutationRate = static_cast<double>(mutationRate.GetValue());

    for ( auto & g : m_aGeneGeneral )
    {
        g.m_gene.Mutate( dMutationRate, m_aLimitsGeneral[ static_cast<int>( g.m_type ) ], random );
    }                                                                                                                     // verified in assembly code  

    for ( auto & g : m_aGeneActions )
    {
        g.m_gene.Mutate( dMutationRate, m_aLimitsActions[ static_cast<int>( g.m_type ) ], random );
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
        short  sNewAllele = genome.m_aGeneActions[ static_cast<int>( g.m_type ) ].m_gene.GetAllele( );
        g.m_gene.SetAllele( sNewAllele );
    }
}

Action::Id Genome::GetOption
( 
    bool           const bHasFreeSpace, 
    bool           const bHasNeighbor,
    ENERGY_UNITS   const energy,
	EVO_GENERATION const age,
    Random             & random 
) const
{
	if (
		  ( enabled( Action::Id::passOn ) ) && 
		  ( m_mortalityTable[ age.GetValue() ] > random.NextRandomNumber() )
	   )
	{
		return Action::Id::passOn;
	}
	else
	{
		std::array <bool, Action::COUNT> abOptions;  // passOn is no longer an option here
 
		abOptions[ static_cast<int>( Action::Id::move      ) ] = bHasFreeSpace &&                 ( energy.GetValue() >= GetAllele( GeneralGeneType::Id::thresholdMove )      );
		abOptions[ static_cast<int>( Action::Id::fertilize ) ] =                                  ( energy.GetValue() >= GetAllele( GeneralGeneType::Id::thresholdFertilize ) );
		abOptions[ static_cast<int>( Action::Id::clone     ) ] = bHasFreeSpace &&                 ( energy.GetValue() >= GetAllele( GeneralGeneType::Id::thresholdClone )     );
		abOptions[ static_cast<int>( Action::Id::marry     ) ] = bHasFreeSpace && bHasNeighbor && ( energy.GetValue() >= GetAllele( GeneralGeneType::Id::thresholdMarry )     );
		abOptions[ static_cast<int>( Action::Id::interact  ) ] =                  bHasNeighbor;
		abOptions[ static_cast<int>( Action::Id::eat       ) ] =                                  ( energy.GetValue() <  GetAllele( GeneralGeneType::Id::maxEat )             );

		unsigned int uiSum = 0;

		for ( auto & gene : m_aGeneActions )
		{
			int index = static_cast<int>( gene.m_type );
			if ( abOptions.at( index ) && m_abActionEnabled.at( index ) )
			{
				short const sVal = gene.m_gene.GetAllele( );
				assert( sVal >= 0 );
				//lint -e571  suspicious cast
				unsigned int const uiVal = static_cast<unsigned int>( sVal );
				//lint +e571
				uiSum += uiVal;
			}
		}

		int iVal = random.NextRandomNumberScaledTo( uiSum );

		for ( auto & gene : m_aGeneActions )
		{
			int index = static_cast<int>( gene.m_type );
			if ( abOptions.at( index ) )
				iVal -= gene.m_gene.GetAllele( );
			if ( iVal < 0 )
			{
				assert( abOptions.at( index ) );
				return GetRelatedAction( gene.m_type );
			}
		}

		return Action::Id::undefined;
	}
}
