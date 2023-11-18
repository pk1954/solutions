// dump.h : 
//

module;

#include <fstream>
#include <iostream>

export module DUMP;

import GridPoint;
import GridCoord;

class Grid;
class GridField;

class DUMP
{
public:
    static void SetDumpStream(std::wostream * const);

    static void DumpNL();
    static void Dump(wchar_t const *);
    static void Dump(GridField const &);
    static void Dump(Grid const &, GridPoint const);
    static void Flush();

private:
    static std::wostream * m_pDumpStream;
};
