// EvolutionTypes.h
//

#pragma once

#include "debug.h"
#include "NamedType.h"

//typedef unsigned short MEM_INDEX;

using MEM_INDEX = NamedType< unsigned short, struct MEM_INDEX_Parameter >;

typedef          long  EVO_GENERATION;

static int const IMEMSIZE_MAX = 9;

enum class tStrategyId : unsigned short
{ 
    defect, 
    cooperate, 
    tit4tat, 
//	friedman, 
    nrOfStrategies, 
    random,         // Not really a strategy. Used to signal random fill
    empty,          // Not really a strategy. Used to mark empty slot
    all             // Not really a strategy. Used to mark all strategies
};

static const int NR_STRATEGIES = static_cast<int>( tStrategyId::nrOfStrategies );

enum class tShape : unsigned short
{
    Circle,
    Rect,
	Grid
};

enum class tOrigin : unsigned short
{
    editor,
    cloning,
    marriage
};

enum class tAction : unsigned short
{ 
    move, 
    clone,
    marry,
    interact, 
    eat, 
    fertilize,
	nrOfActionGenes,
	passOn = nrOfActionGenes,
    nrOfActions,
    undefined = nrOfActions
};

static const int NR_ACTIONS      = static_cast<int>( tAction::nrOfActions );
static const int NR_ACTION_GENES = static_cast<int>( tAction::nrOfActionGenes );

enum class tGeneType : unsigned short
{
    appetite,
    fertilInvest,
    memSize,
    thresholdClone,       // minimum available energy for considering CLONE
    thresholdMarry,       // minimum available energy for considering MARRY
    thresholdMove,        // minimum available energy for considering MOVE
    thresholdFertilize,   // minimum available energy for considering FERTILIZE
    maxEat,               // maximum available energy for considering EAT
    cloneDonation,        // amount of energy donated to clone. 0 means nothing (clone dies), SHORT_MAX means all available energy (parent dies)
    nrOfGeneTypes
};

static const int NR_GENES = static_cast<int>( tGeneType::nrOfGeneTypes );

enum class tBrushMode
{
    randomStrat,
    cooperate,
    defect,
    tit4tat,
    noAnimals,
    move,
    mutRate,
    fertility,
    food,
    fertilizer
};

bool IsStrategyBrushMode( tBrushMode const );

enum class tManipulator
{
	set,
	max,
	min,
	add,
	subtract,
	mean
};

enum class tColorObject : unsigned short
{
	individual,
	selection,
	highlight
};

wchar_t const * const GetStrategyName      ( tStrategyId  const );
wchar_t const * const GetShapeName         ( tShape       const );
wchar_t const * const GetOriginName        ( tOrigin      const );
wchar_t const * const GetActionTypeName    ( tAction      const );
wchar_t const * const GetGeneName          ( tGeneType    const );
wchar_t const * const GetBrushModeName     ( tBrushMode   const );
wchar_t const * const GetBrushModeNameShort( tBrushMode   const );
wchar_t const * const GetManipulatorName   ( tManipulator const );
wchar_t const * const GetColorObjectName   ( tColorObject const );

tAction const GetRelatedAction( tGeneType const );
