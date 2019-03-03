// EvolutionTypes.h
//

#pragma once

#include <functional>
#include "debug.h"
#include "NamedType.h"

using EVO_GENERATION = NamedType< long,           struct EVO_GENERATION_Parameter >;
using IND_ID         = NamedType< long,           struct IND_ID_Parameter >;
using MEM_INDEX      = NamedType< unsigned short, struct MEM_INDEX_Parameter >;

static int const IMEMSIZE_MAX = 9;

class StrategyData;

class Strategy
{
public:
	enum class Id : unsigned short
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

	static const int NR_STRATEGIES = static_cast<int>( Id::nrOfStrategies );

	virtual Id   GetStrategyId( )                                         const = 0;
    virtual bool InteractWith( StrategyData &, IND_ID const )             const = 0;
    virtual void Remember    ( StrategyData &, IND_ID const, bool const ) const = 0;

	static wchar_t const * const GetName( Strategy::Id const );

	static void Apply2All( std::function<void(unsigned short const &)> const & func )
	{
        for ( unsigned short usStrategy = 0; usStrategy < Strategy::NR_STRATEGIES; ++usStrategy )
		{
            func( usStrategy );
		}
	}

	static void Apply2All( std::function<void(Id const &)> const & func )
	{
        for ( unsigned short usStrategy = 0; usStrategy < Strategy::NR_STRATEGIES; ++usStrategy )
		{
            func( static_cast<Strategy::Id>(usStrategy) );
		}
	}
};

class Action
{
public:
	enum class Id : unsigned short
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

	static const int NR_ACTIONS      = static_cast<int>( Action::Id::nrOfActions );
	static const int NR_ACTION_GENES = static_cast<int>( Action::Id::nrOfActionGenes );

	static wchar_t const * const GetName( Action::Id const );
};

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

wchar_t const * const GetShapeName         ( tShape       const );
wchar_t const * const GetOriginName        ( tOrigin      const );
wchar_t const * const GetGeneName          ( tGeneType    const );
wchar_t const * const GetBrushModeName     ( tBrushMode   const );
wchar_t const * const GetBrushModeNameShort( tBrushMode   const );
wchar_t const * const GetManipulatorName   ( tManipulator const );
wchar_t const * const GetColorObjectName   ( tColorObject const );

Action::Id const GetRelatedAction( tGeneType const );
