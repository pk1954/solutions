// Individual.cpp
//
// EvoCoreLib

module EvoCoreLib:;

import Debug;

module EvoCoreLib:Individual;

import Random;
import :EvoConfig;
import :Strategy;

static DefectAlways    StratD;
static CooperateAlways StratC;
static Tit4Tat         StratT;
static EmptyStrategy   StratNull;

ENERGY_UNITS Individual::m_stdEnergyCapacity;
ENERGY_UNITS Individual::m_initialEnergy;

const std::array< Strategy * const, Strategy::COUNT > Individual::m_apStrat =
{ 
	&StratD, // Strategy::Id::defect,   
	&StratC, // Strategy::Id::cooperate,
	&StratT  // Strategy::Id::tit4tat,  
};

void Individual::RefreshCache()
{
    m_stdEnergyCapacity = ENERGY_UNITS(EvoConfig::GetConfigValueShort(EvoConfig::tId::stdCapacity));
    m_initialEnergy     = ENERGY_UNITS(EvoConfig::GetConfigValueShort(EvoConfig::tId::initialEnergy));
}

Individual::Individual()
{ 
    ResetIndividual();
}

void Individual::ResetIndividual()
{ 
    m_id.Set2Null(); 
    m_genBirth.Set2Null();
    m_origin     = tOrigin::undefined;
    m_enStock    = 0_ENERGY_UNITS;
    m_enCapacity = 0_ENERGY_UNITS;
    m_strategyId = Strategy::Id::empty;
    m_stratData.SetMemorySize(0);
    m_genome.InitGenome();
};

void Individual::Create
(
    IND_ID         const id,
    EVO_GENERATION const genBirth,
    Strategy::Id   const strategyId
)
{
    m_genome.InitGenome();
    m_id         = id;
    m_genBirth   = genBirth;
    m_origin     = tOrigin::editor;
    m_enCapacity = m_stdEnergyCapacity;
    m_strategyId = strategyId;
    m_stratData.SetMemorySize(m_genome.GetAllele(GeneType::Id::memSize));  // clears memory. Experience not inheritable.
    SetEnergy(m_initialEnergy); // makes IsAlive() true. Last assignment to avoid race conditions  
}

// Clone - creates a mutated clone of this individual
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
    m_strategyId = indParent.m_strategyId;
    m_genome.Mutate(mutationRate, random);
    m_stratData.SetMemorySize(m_genome.GetAllele(GeneType::Id::memSize));  // clears memory. Experience not inheritable.
}

static Individual const & selectParent
(
    Random           & random,
    Individual const & indParA,
    Individual const & indParB
)
{
    return random.NextBooleanValue() ? indParA : indParB;
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
    m_enCapacity = selectParent(random, indParentA, indParentB).m_enCapacity;
    m_strategyId = selectParent(random, indParentA, indParentB).m_strategyId;
    m_genome.Recombine(indParentA.m_genome, indParentB.m_genome, random);
    m_genome.Mutate(mutationRate, random);
    m_stratData.SetMemorySize(m_genome.GetAllele(GeneType::Id::memSize));  // clears memory. Experience not inheritable.
}
