// EvolutionTypes.h
//
// EvolutionCoreInterface

#pragma once

#include <functional>
#include "util.h"
#include "debug.h"
#include "action.h"
#include "GeneType.h"
#include "NamedType.h"

using ENERGY_UNITS   = NamedType< short, struct ENERGY_UNITS_Parameter >;
using EVO_GENERATION = NamedType< long,  struct EVO_GENERATION_Parameter >;
using IND_ID         = NamedType< long,  struct IND_ID_Parameter >;
using MEM_INDEX      = NamedType< short, struct MEM_INDEX_Parameter >;
using ACTION_COUNT   = NamedType< long,  struct ACTION_COUNT_Parameter >;
using PERCENT        = NamedType< short, struct PERCENT_Parameter >;

constexpr PERCENT operator"" _PERCENT( unsigned long long ull )
{
	return PERCENT( CastToShort( ull ) );
}

constexpr ENERGY_UNITS operator"" _ENERGY_UNITS( unsigned long long ull )
{
	return ENERGY_UNITS( CastToShort( ull ) );
}

static int const IMEMSIZE_MAX = 9;

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

Action::Id   const GetRelatedAction( GeneType::Id const );
GeneType::Id const GetRelatedGeneType( Action::Id const );
