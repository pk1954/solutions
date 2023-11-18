// EvolutionCoreImpl.cpp
//

#include <cassert>
#include <array>
#include "strategy.h"
#include "grid_model.h"
#include "gplIterator.h"
#include "EvolutionCoreWrappers.h"
#include "EvolutionCoreImpl.h"

import Config;
import EvolutionDump;

EvolutionCoreImpl::EvolutionCoreImpl() :
	m_brush(& m_grid)
{ 
	ResetAll();
};

EvolutionCoreImpl::~EvolutionCoreImpl() { }

void EvolutionCoreImpl::ResetAll()
{
    m_grid.ResetGrid();
	m_brush.Reset();
	GridPOI::ClearPoi();
}

// Compute - plan and implement one generation step for all living individuals
//           let food grow 
//           increment generation number

bool EvolutionCoreImpl::Compute()
{
    GplIterator m_gplIterator(m_grid);
    GridPoint   gpRun = m_gplIterator.Begin();

	m_grid.PrepareComputation();
    while (gpRun.IsNotNull())
    {
        assert(IsInGrid(gpRun));
        assert(m_grid.IsAlive(gpRun));

		gpRun = m_grid.ComputeNextGeneration(gpRun);   // may return NULL_VAL  
    }

    m_grid.FoodGrowth();
    m_grid.IncGenNr();

    return false;
}

void EvolutionCoreImpl::DumpGridPointList() const
{
    int iCount = 0;
    EvolutionDump::Dump(L"#  +++ start dump of GridPoint list");
	EvolutionDump::DumpNL();
    GplIterator iter(m_grid);
    for ((void)iter.Begin(); iter.IsNotAtEnd(); (void)iter.GotoNext())
    {
		GridField gf = m_grid.GetGridField(iter.GetCurrent());
        EvolutionDump::Dump(m_grid, iter.GetCurrent());
        EvolutionDump::Dump(L"");
        if (++iCount >= 1000)
        {
            EvolutionDump::Dump(L"# +++ dumpGridPointList loop counter exceeded ");
            break;
        }
    }
    EvolutionDump::Dump(L"#  +++ end of dump");
	EvolutionDump::DumpNL();
}
	