// EvoHistorySys.h
//

#pragma once

#include "HistCacheItem.h"
#include "HistorySystem.h"
#include "EvoModelData.h"
#include "EvolutionCore.h"

class EvolutionCore;

class EvoHistorySys
{
public:

    EvoHistorySys( NextGenFunctor * const, EvoModelData * const );
    ~EvoHistorySys( ) { };

    // EvoApproachHistGen - Get closer to demanded HIST_GENERATION
    //                    - If several steps are neccessary, function returns after one displayed generation
    //                      to allow user interaction
    //                    - But actual history generation as altererd by at least 1

    void EvoApproachHistGen( HIST_GENERATION const genDemanded ) { m_HistorySystem.ApproachHistGen( genDemanded ); }
    
    bool AddEvoHistorySlot    ( ) const                   { return m_HistorySystem.AddHistorySlot( ); }
    int  GetNrOfHistCacheSlots( )                         { return m_HistorySystem.GetNrOfHistCacheSlots( ); }
    void ShutDownHistCacheSlot( int const i )             { return m_HistorySystem.ShutDownHistCacheSlot( i ); }

	bool CreateNewGeneration( tEvoCmd cmd, short sParam ) 
	{ 
		return m_HistorySystem.CreateNewGeneration( static_cast< unsigned short >(cmd), sParam ); 
	}

    HIST_GENERATION GetCurrentGeneration( ) const { return m_pHistCacheItem->GetHistGenCounter( ); }

    EvoModelData  * GetEvoModelData ( ) { return   m_pEvoModelWork; }
    HistorySystem * GetHistorySystem( ) { return & m_HistorySystem; }
    
    HIST_GENERATION GetFirstGenOfIndividual( IndId const & ) const;
    HIST_GENERATION GetLastGenOfIndividual ( IndId const & ) const;

private:
    EvoModelData  * const m_pEvoModelWork;
	HistCacheItem *       m_pHistCacheItem; 
    HistorySystem         m_HistorySystem;

    // private member functions

    bool             IsInHistoryMode( )       const { return m_HistorySystem.IsInHistoryMode( ); }
    HIST_GENERATION  GetYoungestGeneration( ) const { return m_HistorySystem.GetYoungestGeneration( ); }
    HIST_GENERATION  GetNrOfGenerations( )    const { return m_HistorySystem.GetNrOfGenerations( ); }
};
