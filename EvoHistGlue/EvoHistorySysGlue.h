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
	virtual ~EvoHistorySysGlue( ) { };

	EvoModelDataGlue * Start
	( 
		HistorySystem     * const,
		ObserverInterface * const
	);

	void Stop( );

    // EvoApproachHistGen - Get closer to demanded HIST_GENERATION
    //                    - If several steps are neccessary, function returns after one displayed generation
    //                      to allow user interaction
    //                    - But actual history generation is alterered by at least 1

    ModelData const * EvoApproachHistGen( HIST_GENERATION const genDemanded ) { return m_pHistorySystem->ApproachHistGen( genDemanded ); } 
	
    HistSlotNr      GetNrOfUsedHistCacheSlots( ) const { return m_pHistorySystem->GetNrOfUsedHistCacheSlots( ); }
    HistSlotNr      GetNrOfHistCacheSlots    ( ) const { return m_pHistorySystem->GetNrOfHistCacheSlots( ); }
    HIST_GENERATION GetCurrentGeneration     ( ) const { return m_pHistorySystem->GetCurrentGeneration( ); }
    HIST_GENERATION GetYoungestGeneration    ( ) const { return m_pHistorySystem->GetYoungestGeneration( ); }

	HIST_GENERATION GetGenWithIndividual( GridPoint const, bool const ) const; 

	void EvoClearHistory( );
	void EvoClearHistory( HIST_GENERATION const genFirst ) { return m_pHistorySystem->ClearHistory( genFirst ); }

	EvolutionCore const * GetEvolutionCore( HIST_GENERATION const ) const;

	ModelData const * EvoCreateNextGenCommand( )
	{ 
	 	return m_pHistorySystem->CreateAppCommand( NEXT_GEN_CMD );  
	}  

	ModelData const * GetModelData( HIST_GENERATION const gen ) const
	{
		return m_pHistorySystem->GetModelData( gen );
	}

	bool IsEditorCommand( HIST_GENERATION const gen ) const
	{
		return ::IsEditorCommand( static_cast<tEvoCmd>( m_pHistorySystem->GetGenerationCmd( gen ) ) );
	}

	void EvoCreateEditorCommand( GenerationCmd const );

	static GenerationCmd EvoCmd( tEvoCmd const cmd, Int24 const param )
	{ 
		return GenerationCmd::ApplicationCmd( static_cast<tGenCmd>(cmd), param );  
	}  

private:
    static GenerationCmd const NEXT_GEN_CMD;
	
	EvoModelFactory * m_pEvoModelFactory;
    HistorySystem   * m_pHistorySystem;
	HistAllocThread * m_pHistAllocThread;
};
