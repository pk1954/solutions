// dump.cpp
//

#include <fstream>
#include <iomanip>
#include "grid_model.h"
#include "dump.h"

using std::wostream;
using std::wcout;
using std::endl;

wostream * DUMP::m_pDumpStream = & wcout;

void DUMP::SetDumpStream(wostream * const pDumpStream) 
{
    m_pDumpStream = pDumpStream;
};

void DUMP::Flush() 
{
    (void)m_pDumpStream->flush();
};

void DUMP::DumpNL()
{
    *m_pDumpStream << endl;
}

void DUMP::Dump(wchar_t const * const str)
{
    *m_pDumpStream << str;
}

void DUMP::Dump(GridField const & gf)
{
	*m_pDumpStream << gf.GetGridPoint();
	*m_pDumpStream << L" en=" << std::setw(8) << gf.GetEnergy().GetValue();
	*m_pDumpStream << endl;
}

void DUMP::Dump(Grid const & grid, GridPoint const gp)
{
    Dump(grid.GetGridField(gp));
}
