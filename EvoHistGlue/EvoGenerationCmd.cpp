// EvoGenerationCmd.cpp
//

#include "stdafx.h"
#include <unordered_map>
#include <fstream>
#include "EvoGenerationCmd.h"

using namespace std;

bool IsEditorCommand( tEvoCmd const cmd )
{
	static_assert( (int)tEvoCmd::LAST_APP_CMD < GenerationCmd::MAX_APP_CMD, "Too many tEvoCmd values" );

	return ( tEvoCmd::setSimulationMode <= cmd ) && ( cmd <= tEvoCmd::editSetPOI );
}

wchar_t const * const GetEvoCommandNameShort( tEvoCmd const cmd )
{
    static unordered_map < tEvoCmd, wchar_t const * const > mapNames =
    {
        { tEvoCmd::nextGen,               L"NEXT"     },
        { tEvoCmd::reset,                 L"RESET"    },  
        { tEvoCmd::setSimulationMode,     L"SIMU"     },
        { tEvoCmd::editSetBrushMode,      L"MODE"     },
        { tEvoCmd::editSetBrushShape,     L"SHAPE"    },
        { tEvoCmd::editSetBrushOperator,  L"OPERATOR" },
        { tEvoCmd::editSetBrushSize,      L"SIZE"     },
        { tEvoCmd::editSetBrushIntensity, L"INTENS"   },
        { tEvoCmd::editDoEdit,            L"EDIT"     },
        { tEvoCmd::editSetPOI,            L"SETPOI"   }
    };

    return mapNames.at( cmd );
}

wchar_t const * const GetEvoCommandName( tEvoCmd const cmd )
{
    static unordered_map < tEvoCmd, wchar_t const * const > mapNames =
    {
        { tEvoCmd::nextGen,               L"tEvoCmd::nextGen" },
        { tEvoCmd::reset,                 L"tGenCmd::reset" },
        { tEvoCmd::setSimulationMode,     L"tEvoCmd::setSimulationMode"  },
        { tEvoCmd::editSetBrushMode,      L"tEvoCmd::editSetBrushMode" },
        { tEvoCmd::editSetBrushShape,     L"tEvoCmd::editSetBrushShape" },
        { tEvoCmd::editSetBrushOperator,  L"tEvoCmd::editSetBrushOperator" },
        { tEvoCmd::editSetBrushSize,      L"tEvoCmd::editSetBrushSize" },
        { tEvoCmd::editSetBrushIntensity, L"tEvoCmd::editSetBrushIntensity" },
        { tEvoCmd::editDoEdit,            L"tEvoCmd::editDoEdit" },
        { tEvoCmd::editSetPOI,            L"tEvoCmd::editSetPOI" }
    };

    return mapNames.at( cmd );
}

wostream & operator << ( wostream & out, tEvoCmd const & cmd )
{
	if ( GenerationCmd::IsAppCmd( static_cast<tGenCmd>(cmd) ) )
		out << GetEvoCommandNameShort( cmd  );
	else 
		out << GetGenerationCmdNameShort( static_cast<tGenCmd>(cmd) );
    return out;
};
