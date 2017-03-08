// genome.cpp : 
//

#include "stdafx.h"
#include <array>
#include "assert.h"
#include "config.h"
#include "strategy.h"
#include "genome.h"
#include "random.h"

//lint  -e1566    initialization 

// static members and functions

Genome Genome::m_genomeTemplate;

array< GeneTypeLimits, NR_GENES   > Genome::m_aLimitsGeneral;
array< GeneTypeLimits, NR_ACTIONS > Genome::m_aLimitsActions;

bool Genome::m_bMoveEnabled;
bool Genome::m_bFertilizeEnabled;
bool Genome::m_bCloneEnabled;
bool Genome::m_bMarryEnabled;
bool Genome::m_bInteractEnabled;
bool Genome::m_bEatEnabled;

void Genome::setGeneralLimits( tGeneType const gene, long const lLo, long const lHi )
{
    m_aLimitsGeneral[ static_cast<int>( gene ) ].SetLimits( lLo, lHi );
}

void Genome::InitClass( )
{ 
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

    m_bMoveEnabled      = Config::GetConfigValue( Config::tId::moveEnabled      ) > 0;
    m_bFertilizeEnabled = Config::GetConfigValue( Config::tId::fertilizeEnabled ) > 0;
    m_bCloneEnabled     = Config::GetConfigValue( Config::tId::cloneEnabled     ) > 0;
    m_bMarryEnabled     = Config::GetConfigValue( Config::tId::marryEnabled     ) > 0;
    m_bInteractEnabled  = Config::GetConfigValue( Config::tId::interactEnabled  ) > 0;
    m_bEatEnabled       = Config::GetConfigValue( Config::tId::eatEnabled       ) > 0;
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
    for ( auto & g : m_aGeneGeneral )
    {
        g.m_gene.Mutate( sMutationRate, m_aLimitsGeneral[ static_cast<int>( g.m_type ) ], random );
    }                                                                                                                     // verified in assembly code  

    for ( auto & g : m_aGeneActions )
    {
        g.m_gene.Mutate( sMutationRate, m_aLimitsActions[ static_cast<int>( g.m_action ) ], random );
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
    bool const bHasFreeSpace, 
    bool const bHasNeighbor,
    int  const iEnergy,
    Random   & random 
) const
{
    int iNrOfOptions = 0;

    array <bool, NR_ACTIONS > abOptions;
    //lint -e720     Boolean test of assignment
    if ( abOptions[ static_cast<int>( tAction::move      ) ] = m_bMoveEnabled      && bHasFreeSpace &&                 ( iEnergy >= GetAllele( tGeneType::thresholdMove )       ) ) ++iNrOfOptions;
    if ( abOptions[ static_cast<int>( tAction::fertilize ) ] = m_bFertilizeEnabled &&                                  ( iEnergy >= GetAllele( tGeneType::thresholdFertilize ) ) ) ++iNrOfOptions;
    if ( abOptions[ static_cast<int>( tAction::clone     ) ] = m_bCloneEnabled     && bHasFreeSpace &&                 ( iEnergy >= GetAllele( tGeneType::thresholdClone )      ) ) ++iNrOfOptions;
    if ( abOptions[ static_cast<int>( tAction::marry     ) ] = m_bMarryEnabled     && bHasFreeSpace && bHasNeighbor && ( iEnergy >= GetAllele( tGeneType::thresholdMarry )      ) ) ++iNrOfOptions;
    if ( abOptions[ static_cast<int>( tAction::interact  ) ] = m_bInteractEnabled  && bHasNeighbor )                                                                          ++iNrOfOptions;
    if ( abOptions[ static_cast<int>( tAction::eat       ) ] = m_bEatEnabled       && ( iNrOfOptions == 0 ) ||         ( iEnergy <  GetAllele( tGeneType::maxEat )        ) ) ++iNrOfOptions;
    //lint -e720

    unsigned int uiSum = 0;

    for ( auto & g : m_aGeneActions )
    {
        int index = static_cast<int>( g.m_action );
        if ( abOptions[ index ] )
        {
            short const sVal = g.m_gene.GetAllele( );
            assert( sVal >= 0 );
            //lint -e571  suspicious cast
            unsigned int const uiVal = static_cast<unsigned int>( sVal );
            //lint +e571
            uiSum += uiVal;
        }
    }

    unsigned int const uiRand   = random.NextRandomNumber( );
    unsigned int const uiFaktor = uiRand * uiSum;
    int iVal = uiFaktor >> 15;

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
    assert( false );
    return static_cast<tAction>( 0 );
}

short Genome::GetDistr( tAction const action ) const
{ 
    return m_aGeneActions.at( static_cast<int>( action ) ).m_gene.GetAllele();
}
