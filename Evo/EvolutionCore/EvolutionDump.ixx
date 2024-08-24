// EvolutionDump.ixx 
//
// EvoCoreLib

export module EvoCoreLib:EvolutionDump;

import std;
import :GridPoint;
import :GridCoord;
import :GridModel;
import :GridField;

export class EvolutionDump
{
public:
    static void SetDumpStream(std::wostream * const);

    static void DumpNL();
    static void Dump(wchar_t const *);
    static void Dump(GridField const &);
    static void Dump(GridModel const &, GridPoint const);
    static void Flush();

private:
    static std::wostream * m_pDumpStream;
};
