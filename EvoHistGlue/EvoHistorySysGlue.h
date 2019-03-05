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

class RootWindow;
class EvoModelFactory;
class HistAllocThread;
class EvolutionCore;
class ObserverInterface;

class EvoHistorySysGlue
{
public:

    EvoHistorySysGlue( );
	virtual ~EvoHistorySysGlue( );

    void Start
	( 
		EvolutionCore * const, 
		HistorySystem * const,
		long const, 
		bool const,
		RootWindow * const
	);

    // EvoApproachHistGen - Get closer to demanded HIST_GENERATION
    //                    - If several steps are neccessary, function returns after one displayed generation
    //                      to allow user interaction
    //                    - But actual history generation is alterered by at least 1

    void EvoApproachHistGen( HIST_GENERATION const genDemanded ) { m_pHistorySystem->ApproachHistGen( genDemanded ); } 
	
    HistSlotNr      GetNrOfUsedHistCacheSlots( ) const { return m_pHistorySystem->GetNrOfUsedHistCacheSlots( ); }
    HistSlotNr      GetNrOfHistCacheSlots    ( ) const { return m_pHistorySystem->GetNrOfHistCacheSlots( ); }
    HIST_GENERATION GetCurrentGeneration     ( ) const { return m_pHistorySystem->GetCurrentGeneration( ); }
    HIST_GENERATION GetYoungestGeneration    ( ) const { return m_pHistorySystem->GetYoungestGeneration( ); }

	HIST_GENERATION GetGenWithIndividual( IND_ID const &, bool const ) const; 

	void EvoClearHistory( );

	void EvoCreateNextGenCommand( ) 
	{ 
		m_pHistorySystem->CreateAppCommand( NEXT_GEN_CMD );  
	}  

	bool IsEditorCommand( HIST_GENERATION const gen ) const
	{
		return ::IsEditorCommand( static_cast<tEvoCmd>( m_pHistorySystem->GetGenerationCmd( gen ) ) );
	}

	bool EvoCreateEditorCommand( GenerationCmd const );

	static GenerationCmd EvoCmd( tEvoCmd const cmd, Int24 const param )
	{ 
		return GenerationCmd::ApplicationCmd( static_cast<tGenCmd>(cmd), param );  
	}  

private:
    static GenerationCmd const NEXT_GEN_CMD;
	
	EvoModelDataGlue * m_pEvoModelWork;
	EvoModelFactory  * m_pEvoModelFactory;
    HistorySystem    * m_pHistorySystem;
	HistAllocThread  * m_pHistAllocThread;
	bool               m_bAskHistoryCut;

    // private member functions

	bool askHistoryCut( HistorySystem * pHistSys ) const;
    void shutDownHistoryCache( );
};
