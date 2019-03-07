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
static EmptyStrategy   StratNull;

ENERGY_UNITS Individual::m_stdEnergyCapacity;
ENERGY_UNITS Individual::m_initialEnergy;

const std::unordered_map< Strategy::Id, Strategy * const > Individual::m_apStrat =
{ 
	{ Strategy::Id::defect,    &StratD },
	{ Strategy::Id::cooperate, &StratC },
	{ Strategy::Id::tit4tat,   &StratT }
};

void Individual::InitClass( )
{
    m_stdEnergyCapacity = ENERGY_UNITS(Config::GetConfigValueShort( Config::tId::stdCapacity ));
    m_initialEnergy     = ENERGY_UNITS(Config::GetConfigValueShort( Config::tId::initialEnergy ));
}

Individual::Individual( )
{ 
    ResetIndividual( );
}

void Individual::ResetIndividual( )
{ 
    m_id.Set2Null( ); 
    m_genBirth.Set2Null();
    m_origin     = tOrigin::undefined;
    m_enStock    = 0_ENERGY_UNITS;
    m_enCapacity = 0_ENERGY_UNITS;
    m_pStrategy  = & StratNull;
    m_action     = Action::Id::undefined;
    m_stratData.SetMemorySize( MEM_INDEX(0) );
    m_genome.InitGenome( );
};

void Individual::Create
( 
    IND_ID         const id,
    EVO_GENERATION const genBirth,
    Strategy::Id   const strategyId
)
{
    m_genome.InitGenome( );
    m_id         = id;
    m_genBirth   = genBirth;
    m_origin     = tOrigin::editor;
    m_enCapacity = m_stdEnergyCapacity;
    m_pStrategy  = m_apStrat.at( strategyId );
    m_stratData.SetMemorySize( static_cast<MEM_INDEX>(m_genome.GetAllele(GeneType::Id::memSize)) );
    SetEnergy( m_initialEnergy ); // makes IsAlive() true. Last assignment to avoid race conditions  
}

// Clone - creates a mutated clone of this object
//         all member variables of new individual are initialized after this function

void Individual::Clone
(
    IND_ID         const   id,
    EVO_GENERATION const   genBirth,
    PERCENT        const   mutationRate,
    Random               & random,
    Individual     const & indParent
)
{
    m_id         = id;
    m_genBirth   = genBirth;
    m_origin     = tOrigin::cloning;
    m_enCapacity = indParent.m_enCapacity;
    m_pStrategy  = indParent.m_pStrategy;
    m_action     = Action::Id::undefined;
    m_stratData.SetMemorySize( indParent.m_stratData.GetMemSize( ) );  // clears memory. Experience not inheritable.
    m_genome.Mutate( mutationRate, random );
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
    IND_ID         const   id,
    EVO_GENERATION const   genBirth,
    PERCENT        const   mutationRate,
    Random               & random,
    Individual     const & indParentA,
    Individual     const & indParentB
)
{
    m_id         = id;
    m_genBirth   = genBirth;
    m_origin     = tOrigin::marriage;
    m_action     = Action::Id::undefined;
    m_enCapacity = selectParent( random, indParentA, indParentB ).m_enCapacity;
    m_pStrategy  = selectParent( random, indParentA, indParentB ).m_pStrategy;
    m_stratData.SetMemorySize( selectParent( random, indParentA, indParentB ).GetMemSize( ) );  // clears memory. Experience not inheritable.
    m_genome.Recombine( indParentA.m_genome, indParentB.m_genome, random );
    m_genome.Mutate( mutationRate, random );
}
