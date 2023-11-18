// dump.cpp
//

#include <fstream>
#include <iomanip>
#include "grid_model.h"
import EvolutionDump;

using std::wostream;
using std::wcout;
using std::endl;

wostream * EvolutionDump::m_pDumpStream = & wcout;

void EvolutionDump::SetDumpStream(wostream * const pDumpStream) 
{
    m_pDumpStream = pDumpStream;
};

void EvolutionDump::Flush() 
{
    (void)m_pDumpStream->flush();
};

void EvolutionDump::DumpNL()
{
    *m_pDumpStream << endl;
}

void EvolutionDump::Dump(wchar_t const * const str)
{
    *m_pDumpStream << str;
}

void EvolutionDump::Dump(GridField const & gf)
{
	*m_pDumpStream << gf.GetGridPoint();
	*m_pDumpStream << L" en=" << std::setw(8) << gf.GetEnergy().GetValue();
	*m_pDumpStream << endl;
}

void EvolutionDump::Dump(GridModel const & grid, GridPoint const gp)
{
    Dump(grid.GetGridField(gp));
}
