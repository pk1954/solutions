// EvolutionTypes.h
//

#pragma once

#include <functional>
#include "util.h"
#include "debug.h"
#include "NamedType.h"

using ENERGY_UNITS   = NamedType< short,          struct ENERGY_UNITS_Parameter >;
using EVO_GENERATION = NamedType< long,           struct EVO_GENERATION_Parameter >;
using IND_ID         = NamedType< long,           struct IND_ID_Parameter >;
using MEM_INDEX      = NamedType< unsigned short, struct MEM_INDEX_Parameter >;
using ACTION_COUNT   = NamedType< unsigned int,   struct ACTION_COUNT_Parameter >;
using PERCENT        = NamedType< short,          struct PERCENT_Parameter >;

constexpr PERCENT operator"" _PERCENT( unsigned long long ull )
{
	return PERCENT( CastToShort( ull ) );
}

constexpr ENERGY_UNITS operator"" _ENERGY_UNITS( unsigned long long ull )
{
	return ENERGY_UNITS( CastToShort( ull ) );
}

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
		count, 
		random,         // Not really a strategy. Used to signal random fill
		empty,          // Not really a strategy. Used to mark empty slot
		all             // Not really a strategy. Used to mark all strategies
	};

	static const int COUNT = static_cast<int>( Id::count );

	virtual Id   GetStrategyId( )                                         const = 0;
    virtual bool InteractWith( StrategyData &, IND_ID const )             const = 0;
    virtual void Remember    ( StrategyData &, IND_ID const, bool const ) const = 0;

	static wchar_t const * const GetName( Id const );

	static void Apply2All( std::function<void(Id const &)> const & func )
	{
        for ( int index = 0; index < COUNT; ++index )
		{
            func( static_cast<Id>(index) );
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
		count,
		passOn = count,
		nrOfActions,
		undefined = nrOfActions
	};

	static const int NR_ACTIONS = static_cast<int>( Id::nrOfActions );
	static const int COUNT      = static_cast<int>( Id::count );

	static void Apply2All( std::function<void(Id const &)> const & func )
	{
        for ( int index = 0; index < COUNT; ++index )
		{
            func( static_cast<Id>(index) );
		}
	}

	static wchar_t const * const GetName( Id const );
};

class GeneType
{
public:
	enum class Id : unsigned short
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
		count,
		undefined = count
	};

	static const int COUNT = static_cast<int>( Id::count );

	static void Apply2All( std::function<void(Id const &)> const & func )
	{
        for ( int index = 0; index < COUNT; ++index )
		{
            func( static_cast<Id>(index) );
		}
	}

	static wchar_t const * const GetName( Id const );
};

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
    fertilizer,
	undefined
};

bool IsStrategyBrushMode( tBrushMode const );

enum class tManipulator
{
	set,
	max,
	min,
	add,
	subtract,
	mean,
	undefined
};

enum class tColorObject : unsigned short
{
	individual,
	selection,
	highlight,
	undefined
};

enum class tShape : unsigned short
{
    Circle,
    Rect,
	Grid,
	undefined
};

enum class tOrigin : unsigned short
{
    editor,
    cloning,
    marriage,
	undefined
};

wchar_t const * const GetShapeName         ( tShape       const );
wchar_t const * const GetOriginName        ( tOrigin      const );
wchar_t const * const GetBrushModeName     ( tBrushMode   const );
wchar_t const * const GetBrushModeNameShort( tBrushMode   const );
wchar_t const * const GetManipulatorName   ( tManipulator const );
wchar_t const * const GetColorObjectName   ( tColorObject const );

Action::Id const GetRelatedAction( GeneType::Id const );
