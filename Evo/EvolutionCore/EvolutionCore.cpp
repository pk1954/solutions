// EvolutionCore.cpp
//
// EvolutionCore

module EvolutionCore:EvolutionCore;

import std;
import Debug;
import Config;
import :Genome;
import :Strategy;
import GridPOI;
import :GridModel;
import :GridNeighbor;
import EvolutionDump;
import EvolutionCoreWrappers;
import GplIterator;

void EvolutionCore::InitClass
(
	int                 const iNrOfNeighbors,
	ObserverInterface * const pObservers,
	EventInterface    * const pEvent
)
{
    Neighborhood::InitClass(iNrOfNeighbors);
    GridModel::InitClass(pObservers, pEvent);
}

EvolutionCore::EvolutionCore() 
  :	m_brush(& m_grid)
{ 
	ResetAll();
};

EvolutionCore * CreateCore()
{
    return new EvolutionCoreImpl();
}

void EvolutionCore::ResetAll()
{
    m_grid.ResetGrid();
	m_brush.Reset();
	GridPOI::ClearPoi();
}

// Compute - plan and implement one generation step for all living individuals
//           let food grow 
//           increment generation number

bool EvolutionCore::Compute()
{
    GplIterator m_gplIterator(m_grid);
    GridPoint   gpRun = m_gplIterator.Begin();

	m_grid.PrepareComputation();
    while (gpRun.IsNotNull())
    {
        Assert(IsInGrid(gpRun));
        Assert(m_grid.IsAlive(gpRun));

		gpRun = m_grid.ComputeNextGeneration(gpRun);   // may return NULL_VAL  
    }

    m_grid.FoodGrowth();
    m_grid.IncGenNr();

    return false;
}

void EvolutionCore::DestroyModel(EvolutionCore * pCore)
{
	delete pCore;
}

size_t const EvolutionCore::GetCoreSize() 
{ 
	return sizeof(EvolutionCoreImpl) + GetGridHeapSize(); 
};

unsigned int EvolutionCore::GetMaxPartnerMemory()
{ 
	return StrategyData::GetMaxPartnerMemory().GetValue(); 
}

unsigned int EvolutionCore::GetNrInteractionsWithKnownCulprit() 
{ 
	return StrategyData::GetNrInteractionsWithKnownCulprit(); 
}

unsigned int EvolutionCore::GetNrInteractionsWithUnknownCulprit() 
{ 
	return StrategyData::GetNrInteractionsWithUnknownCulprit(); 
}

bool EvolutionCore::IsEnabled(Action::Id const action)
{
	return Genome::IsEnabled(action);
}

std::wostringstream* EvolutionCore::GetProtocolData()
{
	return GridModel::GetProtocolData();
}

void EvolutionCore::ClearProtocolData()
{
	GridModel::ClearProtocolData();
}

void EvolutionCore::DumpGridPointList() const
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
	