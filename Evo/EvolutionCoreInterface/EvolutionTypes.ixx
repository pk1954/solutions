// EvolutionTypes.ixx
//
// EvolutionCoreInterface

module;

#include <functional>
#include <iostream>
#include "action.h"
#include "GeneType.h"

module EvolutionTypes;

import Types;

export using ENERGY_UNITS   = NamedType< short, struct ENERGY_UNITS_Parameter >;
export using EVO_GENERATION = NamedType< long, struct EVO_GENERATION_Parameter >;
export using IND_ID         = NamedType< long, struct IND_ID_Parameter >;
export using MEM_INDEX      = NamedType< short, struct MEM_INDEX_Parameter >;
export using ACTION_COUNT   = NamedType< long, struct ACTION_COUNT_Parameter >;

std::wostream& operator<< (std::wostream& out, IND_ID const&);

export ENERGY_UNITS constexpr operator"" _ENERGY_UNITS(unsigned long long ull)
{
	return ENERGY_UNITS(CastToShort(ull));
}

int constexpr IMEMSIZE_MAX = 9;

export enum class tDisplayMode
{
	normal,
	POI,
	partner,
	target,
	neighbor
};

export enum class tBrushMode : unsigned short
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

export bool IsStrategyBrushMode(tBrushMode const);

export enum class tManipulator : unsigned short
{
	set,
	max,
	min,
	add,
	subtract,
	mean,
	undefined
};

export enum class tColorObject : unsigned short
{
	individual,
	selection,
	highlight,
	undefined
};

export enum class tShape : unsigned short
{
	Circle,
	Rect,
	Grid,
	undefined
};

export enum class tOrigin : unsigned short
{
	editor,
	cloning,
	marriage,
	undefined
};

export wchar_t const* const GetShapeName         (tShape       const);
export wchar_t const* const GetOriginName        (tOrigin      const);
export wchar_t const* const GetBrushModeName     (tBrushMode   const);
export wchar_t const* const GetBrushModeNameShort(tBrushMode   const);
export wchar_t const* const GetManipulatorName   (tManipulator const);
export wchar_t const* const GetColorObjectName   (tColorObject const);

export Action::Id   const GetRelatedAction(GeneType::Id const);
export GeneType::Id const GetRelatedGeneType(Action::Id const);
