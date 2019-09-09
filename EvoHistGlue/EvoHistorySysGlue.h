// EvoHistorySysGlue.h
//
// Glue code for adapting EVOLUTION to HISTORY system
//
// EvoHistorySysGlue is ** above ** HISTORY (calls HISTORY)
//
// EvoHistGlue

#pragma once

#include "HistorySystem.h"
#include "HistAllocThread.h"
#include "EvoModelDataGlue.h"
#include "EvoModelFactory.h"
#include "EvoGenerationCmd.h"

class RootWindow;
class EvoModelFactory;
class EvolutionCore;
class ObserverInterface;

class EvoHistorySysGlue
{
public:

    EvoHistorySysGlue( );
	virtual ~EvoHistorySysGlue( ) { };

	EvoModelDataGlue * Start( HistorySystem * const, BOOL const	);

	void Stop( );

	// EvoApproachHistGen - Get closer to demanded HIST_GENERATION
    //                    - If several steps are neccessary, function returns after one displayed generation
    //                      to allow user interaction
    //                    - But actual history generation is alterered by at least 1

    ModelData const * EvoApproachHistGen( HIST_GENERATION const genDemanded ) { return m_pHistorySystem->ApproachHistGen( genDemanded ); } 
	
	HIST_GENERATION GetGenWithIndividual( GridPoint const, bool const ) const; 

	
	HistorySystem * GetHistorySystem( )
	{
		return m_pHistorySystem;
	}

private:
	EvolutionCore const * getEvolutionCore( HIST_GENERATION const ) const;

    static GenerationCmd const NEXT_GEN_CMD;

	HistAllocThread  m_HistAllocThread;
	EvoModelFactory  m_EvoModelFactory;
	HistorySystem  * m_pHistorySystem;
};
