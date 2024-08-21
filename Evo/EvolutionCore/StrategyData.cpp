// StrategyData.cpp : 
//
// EvolutionCore

module EvolutionCore:;

import Debug;

module EvolutionCore:StrategyData;

MEM_INDEX    StrategyData::m_uiMaxPartnerMemory                 = MEM_INDEX(0);
unsigned int StrategyData::m_uiNrInteractionsWithKnownCulprit   = 0;
unsigned int StrategyData::m_uiNrInteractionsWithUnknownCulprit = 0;

StrategyData::StrategyData()
{
    m_memSize = MEM_INDEX(0);
    m_memUsed = MEM_INDEX(0);
    for	(auto & i : m_aIdCulprits)
        i.Set2Null();
}

void StrategyData::ResetCounters()
{
	m_uiMaxPartnerMemory                 = MEM_INDEX(0);
	m_uiNrInteractionsWithKnownCulprit   = 0;
	m_uiNrInteractionsWithUnknownCulprit = 0;
}

IND_ID StrategyData::GetMemEntry(MEM_INDEX const index) const 
{
	return getCulpritId(index);
}	

MEM_INDEX StrategyData::FindInListOfCulprits(IND_ID const idPartner) 
{
    for	(MEM_INDEX index = MEM_INDEX(0); index < m_memUsed; ++index)
    {
        if (getCulpritId(index) == idPartner)
            return index;
    }

    return MEM_INDEX::NULL_VAL();     
}

void StrategyData::RemoveFromListOfCulprits(MEM_INDEX const index)
{
    Assert(m_memUsed > index);
 
    MEM_INDEX const indexStop = --m_memUsed;
    for	(MEM_INDEX iRun = index; iRun < indexStop; ++iRun)
        setCulpritId(iRun, getCulpritId(iRun + MEM_INDEX(1)));

    setCulpritId(m_memUsed, IND_ID::NULL_VAL());
}

void StrategyData::AddToListOfCulprits(IND_ID const partnerId)
{
    if (m_memUsed == m_memSize)          // list is full. Remove oldest entry
        RemoveFromListOfCulprits(MEM_INDEX(0));

    setCulpritId(m_memUsed++, partnerId);   // add id to list of bad guys
    
	if (m_memUsed > m_uiMaxPartnerMemory)  // statistics of memory usage
        m_uiMaxPartnerMemory = m_memUsed;
}

void StrategyData::SetMemorySize(short const newSize)
{
    Assert(newSize <= IMEMSIZE_MAX);

    m_memSize = static_cast<MEM_INDEX>(newSize);
    m_memUsed = MEM_INDEX(0);
}
