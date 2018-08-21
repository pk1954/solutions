// individual.cpp : 
//

#include "stdafx.h"
#include <unordered_map>
#include "assert.h"
#include "debug.h"
#include "random.h"
#include "config.h"
#include "strategy.h"
#include "individual.h"

static DefectAlways    StratD;
static CooperateAlways StratC;
static Tit4Tat         StratT;

short Individual::m_sStdEnergyCapacity;
short Individual::m_sInitialEnergy;

void Individual::InitClass( )
{
    m_sStdEnergyCapacity = Config::GetConfigValueShort( Config::tId::stdCapacity );
    m_sInitialEnergy     = Config::GetConfigValueShort( Config::tId::initialEnergy );
}

static std::unordered_map< tStrategyId, Strategy * const > apStrat =
{ 
    { tStrategyId::defect,    &StratD },
    { tStrategyId::cooperate, &StratC },
    { tStrategyId::tit4tat,   &StratT }
};

bool Individual::InteractWith( IndId const & partnerId ) 
{ 
    return apStrat.at( m_strategyId )->InteractWith( m_strat, partnerId );
};

void Individual::Remember( IndId const & partnerId, bool const bPartnerReaction ) 
{ 
    apStrat.at( m_strategyId )->Remember( m_strat, partnerId, bPartnerReaction );
};

Individual::Individual( )
  : m_id( ),
    m_strat( ),
    m_genome( )
{ 
    ResetIndividual( );
}

void Individual::ResetIndividual( )
{ 
    m_id.ResetIndId( ); 
    m_genBirth  = -1L;
    m_sEnergy   = 0;
    m_sCapacity = 0;
    m_strat.SetMemorySize( 0 );
    m_strategyId = tStrategyId::empty;
    m_genome.InitGenome( );
};

void Individual::Create
( 
    IndId          const id,
    EVO_GENERATION const genBirth,
    tStrategyId    const s
)
{
    m_strategyId = s;
    m_id         = id;
    m_genBirth   = genBirth;
    m_origin     = tOrigin::editor;
    m_genome.InitGenome( );
    assert( static_cast<int>( m_strategyId ) < NR_STRATEGIES );
    m_strat.SetMemorySize( static_cast<MEM_INDEX>(m_genome.GetAllele(tGeneType::memSize)) );
    m_sCapacity = m_sStdEnergyCapacity;
    SetEnergy( m_sInitialEnergy ); // makes IsAlive() true. Last assignment to avoid race conditions  
}

// Clone - creates a mutated clone of this object
//         all member variables of new individual are initialized after this function

void Individual::Clone
(
    IndId          const   id,
    EVO_GENERATION const   genBirth,
    short          const   sMutationRate,
    Random               & random,
    Individual     const & indParent
)
{
    m_id         = id;
    m_genBirth   = genBirth;
    m_origin     = tOrigin::cloning;
    m_sCapacity  = indParent.m_sCapacity;
    m_strategyId = indParent.m_strategyId;
    m_at         = tAction::undefined;
    m_strat.SetMemorySize( indParent.m_strat.GetMemSize( ) );  // clears memory. Experience not inheritable.
    m_genome.Mutate( sMutationRate, random );
}

static Individual const & selectParent
(
    Random           & random,
    Individual const & indParA,
    Individual const & indParB
)
{
    return random.NextBooleanValue( ) ? indParA : indParB;
}

// Breed - creates a child with a mix of genes of both parents
//         all member variables of new individual are initialized after this function

void Individual::Breed
(
    IndId         const   id,
    EVO_GENERATION const   genBirth,
    short         const   sMutationRate,
    Random              & random,
    Individual    const & indParentA,
    Individual    const & indParentB
)
{
    m_id         =         id;
    m_genBirth   =         genBirth;
    m_origin     =         tOrigin::marriage;
    m_at         =         tAction::undefined;
    m_sCapacity  =         selectParent( random, indParentA, indParentB ).m_sCapacity;
    m_strategyId =         selectParent( random, indParentA, indParentB ).m_strategyId;
    m_strat.SetMemorySize( selectParent( random, indParentA, indParentB ).GetMemSize( ) );  // clears memory. Experience not inheritable.
    m_genome.Recombine( indParentA.m_genome, indParentB.m_genome, random );
    m_genome.Mutate( sMutationRate, random );
}

void Individual::SetEnergy( short const energy )
{
   m_sEnergy = ( energy > m_sCapacity ) ? m_sCapacity : energy;
}

void Individual::IncEnergy( short const sInc )
{
	ASSERT_SHORT_SUM( m_sEnergy, sInc );
    SetEnergy( m_sEnergy + sInc );
}

void Individual::DecEnergy( short const sDec )
{
	ASSERT_SHORT_SUM( m_sEnergy, -sDec );
    SetEnergy( m_sEnergy - sDec );
}
