// EvoHistorySys.h
//
// Glue code for adapting EVOLUTION to HISTORY system
//
// EvoHistorySys is ** above ** HISTORY (calls HISTORY)
//

#pragma once

#include "HistorySystem.h"
#include "EvoModelData.h"
#include "EvoGenerationCmd.h"

class EvoModelFactory;
class HistAllocThread;
class EvolutionCore;

class EvoHistorySys
{
public:

    EvoHistorySys( );
	virtual ~EvoHistorySys( );

    void Start
	( 
		EvolutionModelData * const, 
		EvolutionCore      * const, 
		long const, 
		bool const 
	);

    // EvoApproachHistGen - Get closer to demanded HIST_GENERATION
    //                    - If several steps are neccessary, function returns after one displayed generation
    //                      to allow user interaction
    //                    - But actual history generation is alterered by at least 1

    void EvoApproachHistGen( HIST_GENERATION const genDemanded ) { m_pHistorySystem->ApproachHistGen( genDemanded ); } 
	
    bool            AddEvoHistorySlot    ( ) const { return m_pHistorySystem->AddHistorySlot( ); }
    int             GetNrOfHistCacheSlots( ) const { return m_pHistorySystem->GetNrOfHistCacheSlots( ); }
    HIST_GENERATION GetCurrentGeneration ( ) const { return m_pHistorySystem->GetCurrentGeneration( ); }
    HIST_GENERATION GetYoungestGeneration( ) const { return m_pHistorySystem->GetYoungestGeneration( ); }

    EvoModelData  * GetEvoModelData ( ) { return m_pEvoModelWork;  }
    HistorySystem * GetHistorySystem( ) { return m_pHistorySystem; }
    
    HIST_GENERATION GetFirstGenOfIndividual( IndId const & ) const;
    HIST_GENERATION GetLastGenOfIndividual ( IndId const & ) const;

	void EvoCreateNextGenCommand( ) 
	{ 
		m_pHistorySystem->CreateAppCommand( static_cast< short >(tEvoCmd::nextGen), 0 );  
	}  

	bool EvoCreateEditorCommand( tEvoCmd, unsigned short );

	bool IsEditorCommand( HIST_GENERATION const ) const;

private:
    EvoModelData    * m_pEvoModelWork;
	EvoModelFactory * m_pEvoModelFactory;
    HistorySystem   * m_pHistorySystem;
	HistAllocThread * m_pHistAllocThread;
	bool              m_bAskHistoryCut;

    // private member functions

	bool askHistoryCut( HistorySystem * pHistSys ) const;
    void shutDownHistoryCache( );
};
