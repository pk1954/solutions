// StrategyData.ixx
//
// EvolutionCore

module;

#include <array>

export module StrategyData;

import EvolutionTypes;

export class StrategyData
{
public:
    StrategyData();

	static void ResetCounters();

	void SetMemorySize(short const);

    // display information

    IND_ID    GetMemEntry(MEM_INDEX const) const;	
    MEM_INDEX GetMemSize ()                 const { return m_memSize; };
    MEM_INDEX GetMemUsed ()                 const { return m_memUsed; };

    static MEM_INDEX    GetMaxPartnerMemory()                 { return m_uiMaxPartnerMemory; };
    static unsigned int GetNrInteractionsWithKnownCulprit()   { return m_uiNrInteractionsWithKnownCulprit; };
    static unsigned int GetNrInteractionsWithUnknownCulprit() { return m_uiNrInteractionsWithUnknownCulprit; };

    static void KnownCulprit()   { ++m_uiNrInteractionsWithKnownCulprit; };
    static void UnknownCulprit() { ++m_uiNrInteractionsWithUnknownCulprit; };

	MEM_INDEX FindInListOfCulprits    (IND_ID const);
    void      AddToListOfCulprits     (IND_ID const);
    void      RemoveFromListOfCulprits(MEM_INDEX const);

private:
    static MEM_INDEX    m_uiMaxPartnerMemory;
    static unsigned int m_uiNrInteractionsWithKnownCulprit;
    static unsigned int m_uiNrInteractionsWithUnknownCulprit;

    MEM_INDEX m_memUsed;   // number of occopied slots.     
    MEM_INDEX m_memSize;   // number of usable slots.    m_memUsed <= m_memSize <= IMEMSIZE_MAX

    std::array< IND_ID, IMEMSIZE_MAX > m_aIdCulprits;

	IND_ID getCulpritId(MEM_INDEX const index) const
	{
		return m_aIdCulprits.at(index.GetValue());
	}

	void setCulpritId(MEM_INDEX const index, IND_ID const id)
	{
		m_aIdCulprits.at(index.GetValue()) = id;
	}
};
