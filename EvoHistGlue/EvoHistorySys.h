// EvoHistorySys.h
//

#pragma once

#include "HistorySystem.h"
#include "EvoModelData.h"
#include "EvolutionCore.h"

class EvolutionCore;

class EvoHistorySys
{
public:

    EvoHistorySys( EvoModelFactory * const, EvoModelData * const );
    ~EvoHistorySys( ) { };

    // EvoApproachHistGen - Get closer to demanded HIST_GENERATION
    //                    - If several steps are neccessary, function returns after one displayed generation
    //                      to allow user interaction
    //                    - But actual history generation as altererd by at least 1

    void EvoApproachHistGen( HIST_GENERATION const genDemanded ) { m_pHistorySystem->ApproachHistGen( genDemanded ); }
    
    bool AddEvoHistorySlot    ( ) const                   { return m_pHistorySystem->AddHistorySlot( ); }
    int  GetNrOfHistCacheSlots( )                         { return m_pHistorySystem->GetNrOfHistCacheSlots( ); }
    void ShutDownHistCacheSlot( int const i )             { return m_pHistorySystem->ShutDownHistCacheSlot( i ); }

	bool CreateAppCommand( tEvoCmd cmd, short sParam ) 
	{ 
		return m_pHistorySystem->CreateAppCommand( static_cast< unsigned short >(cmd), sParam ); 
	}

    HIST_GENERATION GetCurrentGeneration( ) const { return m_pHistorySystem->GetCurrentGeneration( ); }

    EvoModelData  * GetEvoModelData ( ) { return m_pEvoModelWork;  }
    HistorySystem * GetHistorySystem( ) { return m_pHistorySystem; }
    
    HIST_GENERATION GetFirstGenOfIndividual( IndId const & ) const;
    HIST_GENERATION GetLastGenOfIndividual ( IndId const & ) const;

private:
    EvoModelData  * const m_pEvoModelWork;
    HistorySystem *       m_pHistorySystem;

    // private member functions

    bool             IsInHistoryMode( )       const { return m_pHistorySystem->IsInHistoryMode( ); }
    HIST_GENERATION  GetYoungestGeneration( ) const { return m_pHistorySystem->GetYoungestGeneration( ); }
    HIST_GENERATION  GetNrOfGenerations( )    const { return m_pHistorySystem->GetNrOfGenerations( ); }
};
