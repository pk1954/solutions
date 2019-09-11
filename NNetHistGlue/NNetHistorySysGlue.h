// NNetHistorySysGlue.h
//
// Glue code for adapting NNetSimu to HISTORY system
//
// NNetHistorySysGlue is ** above ** HISTORY (calls HISTORY)
//
// NNetHistGlue

#pragma once

#include "HistorySystem.h"
#include "HistAllocThread.h"
#include "NNetModelDataGlue.h"
#include "NNetModelFactory.h"
#include "NNetGenerationCmd.h"

class RootWindow;
class NNetModelFactory;
class NNetModel;
class ObserverInterface;

class NNetHistorySysGlue
{
public:

    NNetHistorySysGlue( );
	virtual ~NNetHistorySysGlue( ) { };

	NNetModelDataGlue * Start( HistorySystem * const, BOOL const );

	void Stop( );

	// NNetApproachHistGen - Get closer to demanded HIST_GENERATION
    //                     - If several steps are neccessary, function returns after one displayed generation
    //                       to allow user interaction
    //                     - But actual history generation is alterered by at least 1

    ModelData const * NNetApproachHistGen( HIST_GENERATION const genDemanded ) { return m_pHistorySystem->ApproachHistGen( genDemanded ); } 

	HistorySystem * GetHistorySystem( )
	{
		return m_pHistorySystem;
	}

private:
    static GenerationCmd const NEXT_GEN_CMD;

	HistAllocThread  m_HistAllocThread;
	NNetModelFactory m_NNetModelFactory;
	HistorySystem  * m_pHistorySystem;
};
