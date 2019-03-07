// EvolutionTypes.cpp
//

#include "stdafx.h"
#include <unordered_map>
#include "strategy.h"
#include "EvolutionTypes.h"

using std::unordered_map;

wchar_t const * const Strategy::GetName( Strategy::Id const id )
{
    static unordered_map < Strategy::Id, wchar_t const * const > mapStrategies =
    { 
        { Strategy::Id::defect,    L"D" },
        { Strategy::Id::cooperate, L"C" },
        { Strategy::Id::tit4tat,   L"T" }
    };

    return mapStrategies.at( id );
}

wchar_t const * const GetShapeName( tShape const s )
{
    static unordered_map < tShape, wchar_t const * const > mapShapes =
    {
        { tShape::Circle, L"tShape::Circle"   },
        { tShape::Rect,   L"tShape::Rect" },
        { tShape::Grid,   L"tShape::Grid" }
    };

    return mapShapes.at( s );
}

wchar_t const * const GetOriginName( tOrigin const o )
{
    static unordered_map < tOrigin, wchar_t const * const > mapOrigins =
    {
        { tOrigin::editor,    L" edit" },
        { tOrigin::cloning,   L"clone" },
        { tOrigin::marriage,  L" marr" },
        { tOrigin::undefined, L" undef" }
    };

    return mapOrigins.at( o );
}

wchar_t const * const Action::GetName( Action::Id const action )
{
    static unordered_map < Action::Id, wchar_t const * const > mapOptions =
    { 
        { Action::Id::move,      L"move"      },
        { Action::Id::clone,     L"clone"     },
        { Action::Id::marry,     L"marry"     },
        { Action::Id::interact,  L"interact"  },
        { Action::Id::eat,       L"eat"       },
        { Action::Id::fertilize, L"fertilize" },
        { Action::Id::passOn,    L"passOn"    },
        { Action::Id::undefined, L"undef"     }
    };

    return mapOptions.at( action );
}

wchar_t const * const GeneType::GetName( GeneType::Id const gene )
{
    static unordered_map < GeneType::Id, wchar_t const * const > mapTitles =
    {
        { GeneType::Id::appetite,            L"appetite"  },
        { GeneType::Id::fertilInvest,        L"fert inv"  },
        { GeneType::Id::memSize,             L"mem size"  },
        { GeneType::Id::thresholdClone,      L"min clone" },
        { GeneType::Id::thresholdMarry,      L"min marry" },
        { GeneType::Id::thresholdMove,       L"min move"  },
        { GeneType::Id::thresholdFertilize,  L"min fert"  },
        { GeneType::Id::maxEat,              L"max eat"   },
        { GeneType::Id::cloneDonation,       L"clone don" }
    };

    return mapTitles.at( gene );
}

Action::Id const GetRelatedAction( GeneType::Id const gene )
{
    static unordered_map < GeneType::Id, Action::Id const > mapActions =
    {
        { GeneType::Id::appetite,            Action::Id::eat       },
        { GeneType::Id::fertilInvest,        Action::Id::fertilize },
        { GeneType::Id::memSize,             Action::Id::interact  },
        { GeneType::Id::thresholdClone,      Action::Id::clone     },
        { GeneType::Id::thresholdMarry,      Action::Id::marry     },
        { GeneType::Id::thresholdMove,       Action::Id::move      },
        { GeneType::Id::thresholdFertilize,  Action::Id::fertilize },
        { GeneType::Id::maxEat,              Action::Id::eat       },
        { GeneType::Id::cloneDonation,       Action::Id::clone     }
    };

    return mapActions.at( gene );
}

wchar_t const * const GetBrushModeName( tBrushMode const mode )
{
    static unordered_map < tBrushMode, wchar_t const * const > mapModes =
    {
        { tBrushMode::move,        L"tBrushMode::move" },
        { tBrushMode::randomStrat, L"tBrushMode::randomStrat" },
        { tBrushMode::cooperate,   L"tBrushMode::cooperate" },
        { tBrushMode::defect,      L"tBrushMode::defect" },
        { tBrushMode::tit4tat,     L"tBrushMode::tit4tat" },
        { tBrushMode::noAnimals,   L"tBrushMode::noAnimals" },
        { tBrushMode::mutRate,     L"tBrushMode::mutRate" },
        { tBrushMode::fertility,   L"tBrushMode::fertility" },
        { tBrushMode::food,        L"tBrushMode::food" },
        { tBrushMode::fertilizer,  L"tBrushMode::fertilizer" }
    };

    return mapModes.at( mode );
}

bool IsStrategyBrushMode( tBrushMode const mode ) 
{ 
	return (mode == tBrushMode::randomStrat) 
		|| (mode == tBrushMode::cooperate)
		|| (mode == tBrushMode::defect) 
		|| (mode == tBrushMode::tit4tat) 
		|| (mode == tBrushMode::noAnimals); 
}

wchar_t const * const GetBrushModeNameShort( tBrushMode const mode )
{
    static unordered_map < tBrushMode, wchar_t const * const > mapModes =
    {
        { tBrushMode::move,        L"E_MOV" },
        { tBrushMode::randomStrat, L"E_RAN" },
        { tBrushMode::cooperate,   L"E_COO" },
        { tBrushMode::defect,      L"E_DEF" },
        { tBrushMode::tit4tat,     L"E_T4T" },
        { tBrushMode::noAnimals,   L"E_NOA" },
        { tBrushMode::mutRate,     L"E_MUT" },
        { tBrushMode::fertility,   L"E_FTY" },
        { tBrushMode::food,        L"E_FOO" },
        { tBrushMode::fertilizer,  L"E_FER" }
    };

    return mapModes.at( mode );
}

wchar_t const * const GetManipulatorName( tManipulator const op )
{
    static unordered_map < tManipulator, wchar_t const * const > mapManipulators =
    { 
        { tManipulator::set,      L"set" },
        { tManipulator::max,      L"max" },
        { tManipulator::min,      L"min" },
        { tManipulator::add,      L"add" },
        { tManipulator::subtract, L"subtract" }
    };

    return mapManipulators.at( op );
}

wchar_t const * const GetColorObjectName( tColorObject const object )
{
    static unordered_map < tColorObject, wchar_t const * const > mapColorObjects =
    { 
        { tColorObject::individual, L"individual" },
        { tColorObject::selection,  L"selection" },
        { tColorObject::highlight,  L"highlight" }
    };

    return mapColorObjects.at( object );
}
