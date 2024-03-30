// EvolutionCore.cpp
//
// EvolutionCore

module;

//#include "EvolutionCore.h"
//#include "EvolutionCoreImpl.h"
//#include "EvolutionCoreWrappers.h"

module EvolutionCore;

import Genome;
import Strategy;
import GridPOI;
import GridNeighbor;
import GridModel;

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

EvolutionCore * EvolutionCore::CreateCore()
{
    return new EvolutionCoreImpl();
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
