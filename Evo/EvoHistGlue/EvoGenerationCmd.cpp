// EvoGenerationCmd.cpp
//
// EvoHistGlue

module EvoHistGlueLib:EvoGenerationCmd;

import std;

wchar_t const * const GetEvoCommandNameShort(EvoGenerationCmd::Id const cmd)
{
    static std::unordered_map < EvoGenerationCmd::Id, wchar_t const * const > mapNames =
    {
        { EvoGenerationCmd::Id::editSetBrushMode,        L"MODE"   },
        { EvoGenerationCmd::Id::editSetBrushShape,       L"SHAPE"  },
        { EvoGenerationCmd::Id::editSetBrushManipulator, L"MANIP"  },
        { EvoGenerationCmd::Id::editSetBrushRadius,      L"RADIUS" },
        { EvoGenerationCmd::Id::editSetBrushIntensity,   L"INTENS" },
        { EvoGenerationCmd::Id::editDoEdit,              L"EDIT"   },
        { EvoGenerationCmd::Id::editSetPOI,              L"SETPOI" }
    };

    return mapNames.at(cmd);
}

wchar_t const * const GetEvoCommandName(EvoGenerationCmd::Id const cmd)
{
    static std::unordered_map < EvoGenerationCmd::Id, wchar_t const * const > mapNames =
    {
        { EvoGenerationCmd::Id::editSetBrushMode,        L"EvoGenerationCmd::Id::editSetBrushMode" },
        { EvoGenerationCmd::Id::editSetBrushShape,       L"EvoGenerationCmd::Id::editSetBrushShape" },
        { EvoGenerationCmd::Id::editSetBrushManipulator, L"EvoGenerationCmd::Id::editSetBrushManipulator" },
        { EvoGenerationCmd::Id::editSetBrushRadius,      L"EvoGenerationCmd::Id::editSetBrushRadius" },
        { EvoGenerationCmd::Id::editSetBrushIntensity,   L"EvoGenerationCmd::Id::editSetBrushIntensity" },
        { EvoGenerationCmd::Id::editDoEdit,              L"EvoGenerationCmd::Id::editDoEdit" },
        { EvoGenerationCmd::Id::editSetPOI,              L"EvoGenerationCmd::Id::editSetPOI" }
    };

    return mapNames.at(cmd);
}

std::wostream & operator << (std::wostream & out, EvoGenerationCmd::Id const & cmd)
{
	if (GenerationCmd::IsAppCmd(static_cast<GenerationCmd::Id>(cmd)))
		out << GetEvoCommandNameShort(cmd );
	else 
		out << GetGenerationCmdNameShort(static_cast<GenerationCmd::Id>(cmd));
    return out;
};
