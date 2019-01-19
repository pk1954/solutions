// EvolutionTypes.cpp
//

#include "stdafx.h"
#include <unordered_map>
#include "strategy.h"
#include "EvolutionTypes.h"
#include "gridPoint.h"
#include "gridRect.h"


#include <limits>

using namespace std;

const IndId IndId::NO_INDIVIDUAL = IndId();

std::wostream & operator << ( std::wostream & out, GRID_COORD const c )
{
    out << c.get();
    return out;
}

constexpr GRID_COORD operator"" _GRID_COORD( unsigned long long ull )
{
	return GRID_COORD( CastToUnsignedShort( ull ) );
}

wchar_t const * const GetStrategyName( tStrategyId const id )
{
    static unordered_map < tStrategyId, wchar_t const * const > mapStrategies =
    { 
        { tStrategyId::defect,    L"D" },
        { tStrategyId::cooperate, L"C" },
        { tStrategyId::tit4tat,   L"T" }
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
        { tOrigin::editor,   L" edit"     },
        { tOrigin::cloning,  L"clone"    },
        { tOrigin::marriage, L" marr" }
    };

    return mapOrigins.at( o );
}

wchar_t const * const GetActionTypeName( tAction const action )
{
    static unordered_map < tAction, wchar_t const * const > mapOptions =
    { 
        { tAction::move,      L"move"      },
        { tAction::clone,     L"clone"     },
        { tAction::marry,     L"marry"     },
        { tAction::interact,  L"interact"  },
        { tAction::eat,       L"eat"       },
        { tAction::fertilize, L"fertilize" },
        { tAction::passOn,    L"passOn"    },
        { tAction::undefined, L"undef"     }
    };

    return mapOptions.at( action );
}

wchar_t const * const GetGeneName( tGeneType const gene )
{
    static unordered_map < tGeneType, wchar_t const * const > mapTitles =
    {
        { tGeneType::appetite,            L"appetite"  },
        { tGeneType::fertilInvest,        L"fert inv"  },
        { tGeneType::memSize,             L"mem size"  },
        { tGeneType::thresholdClone,      L"min clone" },
        { tGeneType::thresholdMarry,      L"min marry" },
        { tGeneType::thresholdMove,       L"min move"  },
        { tGeneType::thresholdFertilize,  L"min fert"  },
        { tGeneType::maxEat,              L"max eat"   },
        { tGeneType::cloneDonation,       L"clone don" }
    };

    return mapTitles.at( gene );
}

tAction const GetRelatedAction( tGeneType const gene )
{
    static unordered_map < tGeneType, tAction const > mapActions =
    {
        { tGeneType::appetite,            tAction::eat       },
        { tGeneType::fertilInvest,        tAction::fertilize },
        { tGeneType::memSize,             tAction::interact  },
        { tGeneType::thresholdClone,      tAction::clone     },
        { tGeneType::thresholdMarry,      tAction::marry     },
        { tGeneType::thresholdMove,       tAction::move      },
        { tGeneType::thresholdFertilize,  tAction::fertilize },
        { tGeneType::maxEat,              tAction::eat       },
        { tGeneType::cloneDonation,       tAction::clone     }
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
