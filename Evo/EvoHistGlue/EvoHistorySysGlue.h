// EvoHistorySysGlue.h
//
// Glue code for adapting EVOLUTION to HISTORY system
//
// EvoHistorySysGlue is ** above ** HISTORY (calls HISTORY)
//
// EvoHistGlue

module;

#include "HistorySystem.h"
#include "HistAllocThread.h"
#include "EvoModelDataGlue.h"
#include "EvoModelFactory.h"
module EvoGenerationCmd;

class RootWindow;
class EvoModelFactory;
class EvolutionCore;
class ObserverInterface;

class EvoHistorySysGlue
{
public:

    EvoHistorySysGlue();
	virtual ~EvoHistorySysGlue() { };

	EvoModelDataGlue * Start(HistorySystem * const, bool const	);

	void Stop();

	// EvoApproachHistGen - Get closer to demanded HistGeneration
    //                    - If several steps are neccessary, function returns after one displayed generation
    //                      to allow user interaction
    //                    - But actual history generation is alterered by at least 1

    ModelData const * EvoApproachHistGen(HistGeneration const genDemanded) { return m_pHistorySystem->ApproachHistGen(genDemanded); } 
	
	HistGeneration GetGenWithIndividual(GridPoint const, bool const) const; 

	HistorySystem * GetHistorySystem()
	{
		return m_pHistorySystem;
	}

private:
	EvolutionCore const * getEvolutionCore(HistGeneration const) const;

	HistAllocThread  m_HistAllocThread;
	EvoModelFactory  m_EvoModelFactory;
	HistorySystem  * m_pHistorySystem;
};
