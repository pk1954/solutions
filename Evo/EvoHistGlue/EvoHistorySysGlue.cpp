// EvoHistorySysGlue.cpp
//
// EvoHistGlue

module EvoHistorySysGlue;

import HistGeneration;
import HistorySystem;
import EvoCoreLib;
import WorkThread;

EvoHistorySysGlue::EvoHistorySysGlue() :
    m_pHistorySystem(nullptr)
{}

EvoModelDataGlue * EvoHistorySysGlue::Start
(
	HistorySystem * const pHistorySystem,
	bool            const bAsync      
)
{
	m_pHistorySystem = pHistorySystem;

	ModelData * pModelWork = m_pHistorySystem->StartHistorySystem
    (
        EvoConfig::GetConfigValue(EvoConfig::tId::maxGeneration),
        EvoConfig::GetConfigValue(EvoConfig::tId::nrOfHistorySlots),
		Util::GetPhysicalMemory(),
        & m_EvoModelFactory
   );

	m_HistAllocThread.Start(m_pHistorySystem, bAsync);   // delegate allocation of history slots to a work thread

	return static_cast< EvoModelDataGlue * >(pModelWork);
}

void EvoHistorySysGlue::Stop()
{
	m_HistAllocThread.Terminate();
	m_pHistorySystem->StopHistorySystem();
	m_pHistorySystem = nullptr;
}

class FindGridPointFunctor : public GenerationProperty
{
public:

    FindGridPointFunctor(IND_ID const id) : m_id(id) {}

    virtual bool operator() (ModelData const * pModelData) const
    {
        EvoModelDataGlue const * pEvoModelData = static_cast< EvoModelDataGlue const * >(pModelData);
        return (pEvoModelData->FindGridPointFromId(m_id).IsNotNull());  // id is alive
    }

private:
    IND_ID const m_id;
};

HistGeneration EvoHistorySysGlue::GetGenWithIndividual(GridPoint const gp, bool const bReverse) const
{
	HistGeneration       gen   = m_pHistorySystem->GetCurrentGeneration();
	EvolutionCore const * pCore = getEvolutionCore(gen);
	IND_ID                id    = pCore->GetId(gp);
	return id.IsNull() 
		? HistGeneration()
		: m_pHistorySystem->FindGenerationWithProperty(FindGridPointFunctor(id), bReverse);
}

EvolutionCore const * EvoHistorySysGlue::getEvolutionCore(HistGeneration const gen) const
{
	ModelData const * pModelData { m_pHistorySystem->GetModelData(gen) };
	return (pModelData == nullptr)
	? nullptr  // gen not in cache
	: (static_cast< EvoModelDataGlue const * >(pModelData))->GetEvolutionCoreC();
}
