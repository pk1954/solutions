// EvoHistorySysGlue.h
//
// Glue code for adapting EVOLUTION to HISTORY system
//
// EvoHistorySysGlue is ** above ** HISTORY (calls HISTORY)
//

#pragma once

#include "HistorySystem.h"
#include "EvoModelDataGlue.h"
#include "EvoGenerationCmd.h"

class EvoModelFactory;
class HistAllocThread;
class EvolutionCore;

class EvoHistorySysGlue
{
public:

    EvoHistorySysGlue( );
	virtual ~EvoHistorySysGlue( );

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

    EvoModelDataGlue * GetEvoModelData ( ) { return m_pEvoModelWork;  }
    HistorySystem    * GetHistorySystem( ) { return m_pHistorySystem; }
    
	HIST_GENERATION GetFirstGenOfIndividual( IndId const & ) const; 
	HIST_GENERATION GetLastGenOfIndividual ( IndId const & ) const;  

	GenerationCmd EvoCmd( tEvoCmd const cmd, int16_t const param )
	{ 
		return GenerationCmd::ApplicationCmd( static_cast<tGenCmd>(cmd), param );  
	}  

	void EvoCreateNextGenCommand( ) 
	{ 
		m_pHistorySystem->CreateAppCommand( EvoCmd( tEvoCmd::nextGen, 0 ) );  
	}  

	bool IsEditorCommand( HIST_GENERATION const gen ) const
	{
		return ::IsEditorCommand( static_cast<tEvoCmd>( m_pHistorySystem->GetGenerationCmd( gen ) ) );
	}

	bool EvoCreateEditorCommand( tEvoCmd, int16_t );

private:
    EvoModelDataGlue * m_pEvoModelWork;
	EvoModelFactory  * m_pEvoModelFactory;
    HistorySystem    * m_pHistorySystem;
	HistAllocThread  * m_pHistAllocThread;
	bool               m_bAskHistoryCut;

    // private member functions

	bool askHistoryCut( HistorySystem * pHistSys ) const;
    void shutDownHistoryCache( );
};
