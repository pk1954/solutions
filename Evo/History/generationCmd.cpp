// GenerationCmd.cpp
//
// History

module HistoryLib:GenerationCmd;

import std;

using std::unordered_map;
using std::wostream;

wchar_t const * const GetGenerationCmdNameShort(GenerationCmd::Id const cmd)
{
    static unordered_map < GenerationCmd::Id, wchar_t const * const > mapNames =
    {
		{ GenerationCmd::Id::CACHED,    L"CACHE" },
		{ GenerationCmd::Id::NEXT_GEN,  L"NEXT"  },
        { GenerationCmd::Id::UNDEFINED, L"UNDEF" },
	    { GenerationCmd::Id::RESET,     L"RESET" }
	};

    return mapNames.at(cmd);
}

wchar_t const * const GetGenerationCmdName(GenerationCmd::Id const cmd)
{
    static unordered_map < GenerationCmd::Id, wchar_t const * const > mapNames =
    {
	    { GenerationCmd::Id::CACHED,    L"GenerationCmd::Id::cached"    },
		{ GenerationCmd::Id::NEXT_GEN,  L"GenerationCmd::Id::nextGen"   },
		{ GenerationCmd::Id::UNDEFINED, L"GenerationCmd::Id::undefined" },
		{ GenerationCmd::Id::RESET,     L"GenerationCmd::Id::reset"     }
	};

    return mapNames.at(cmd);
}

wostream & operator << (wostream & out, GenerationCmd const & genCmd)
{
    GenerationCmd::Id cmd = genCmd.GetCommand();
    out << GetGenerationCmdNameShort(cmd);
    return out;
};
