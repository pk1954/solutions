// EvoHistorySys.h
//

#pragma once

#include "HistorySystem.h"
#include "EvoModelData.h"

class EvoModelFactory;
class HistAllocThread;
class StatusBar;
class WorkThread;

class EvoHistorySys
{
public:

    EvoHistorySys( );
	virtual ~EvoHistorySys( );

    void Start( EvolutionModelData * const, WorkThread * const, StatusBar * const );

    // EvoApproachHistGen - Get closer to demanded HIST_GENERATION
    //                    - If several steps are neccessary, function returns after one displayed generation
    //                      to allow user interaction
    //                    - But actual history generation as alterered by at least 1

    void EvoApproachHistGen( HIST_GENERATION const genDemanded ) { m_pHistorySystem->ApproachHistGen( genDemanded ); } // Layer 4
	
    bool            AddEvoHistorySlot    ( ) const { return m_pHistorySystem->AddHistorySlot( ); }
    int             GetNrOfHistCacheSlots( )       { return m_pHistorySystem->GetNrOfHistCacheSlots( ); }
    HIST_GENERATION GetCurrentGeneration ( ) const { return m_pHistorySystem->GetCurrentGeneration( ); }

    EvoModelData  * GetEvoModelData ( ) { return m_pEvoModelWork;  }
    HistorySystem * GetHistorySystem( ) { return m_pHistorySystem; }
    
    HIST_GENERATION GetFirstGenOfIndividual( IndId const & ) const;
    HIST_GENERATION GetLastGenOfIndividual ( IndId const & ) const;

	void EvoCreateResetCommand( ) {	m_pHistorySystem->CreateResetCommand( ); }  // Layer 4
	bool CreateEditorCommand( tEvoCmd, short );

private:
    EvoModelData    * m_pEvoModelWork;
	EvoModelFactory * m_pEvoModelFactory;
    HistorySystem   * m_pHistorySystem;
	HistAllocThread * m_pHistAllocThread;
    StatusBar       * m_pStatusBar;

    // private member functions

	bool             askHistoryCut( HistorySystem * pHistSys ) const;
    void             shutDownHistoryCache( );

    bool             IsInHistoryMode( )       const { return m_pHistorySystem->IsInHistoryMode( ); }
    HIST_GENERATION  GetYoungestGeneration( ) const { return m_pHistorySystem->GetYoungestGeneration( ); }
    HIST_GENERATION  GetNrOfGenerations( )    const { return m_pHistorySystem->GetNrOfGenerations( ); }
};
