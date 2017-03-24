// EvoHistorySys.h
//

#pragma once

#include "HistorySystem.h"
#include "EvoModelData.h"
#include "EvolutionCore.h"

class EvolutionCore;
class EvoModelFactory;

class EvoHistorySys
{
public:

    EvoHistorySys( EvoModelFactory * const, EvoModelData * const );
	virtual ~EvoHistorySys( );

    // EvoApproachHistGen - Get closer to demanded HIST_GENERATION
    //                    - If several steps are neccessary, function returns after one displayed generation
    //                      to allow user interaction
    //                    - But actual history generation as alterered by at least 1

    void EvoApproachHistGen( HIST_GENERATION const genDemanded ) { m_pHistorySystem->ApproachHistGen( genDemanded ); }
	
    bool            AddEvoHistorySlot    ( )           const { return m_pHistorySystem->AddHistorySlot( ); }
    int             GetNrOfHistCacheSlots( )                 { return m_pHistorySystem->GetNrOfHistCacheSlots( ); }
    void            ShutDownHistCacheSlot( int const i )     { return m_pHistorySystem->ShutDownHistCacheSlot( i ); }
    HIST_GENERATION GetCurrentGeneration ( )           const { return m_pHistorySystem->GetCurrentGeneration( ); }

    EvoModelData  * GetEvoModelData ( ) { return m_pEvoModelWork;  }
    HistorySystem * GetHistorySystem( ) { return m_pHistorySystem; }
    
    HIST_GENERATION GetFirstGenOfIndividual( IndId const & ) const;
    HIST_GENERATION GetLastGenOfIndividual ( IndId const & ) const;

	bool CreateEditorCommand( tEvoCmd, short );

private:
    EvoModelData  * const m_pEvoModelWork;
    HistorySystem *       m_pHistorySystem;

    // private member functions

	bool             askHistoryCut( HistorySystem * pHistSys ) const;

    bool             IsInHistoryMode( )       const { return m_pHistorySystem->IsInHistoryMode( ); }
    HIST_GENERATION  GetYoungestGeneration( ) const { return m_pHistorySystem->GetYoungestGeneration( ); }
    HIST_GENERATION  GetNrOfGenerations( )    const { return m_pHistorySystem->GetNrOfGenerations( ); }
};
