// EvoHistorySys.cpp
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include "config.h"
#include "EvolutionCore.h"
#include "win32_util.h"
#include "HistoryGeneration.h"
#include "HistorySystem.h"
#include "EvoHistorySys.h"

EvoHistorySys::EvoHistorySys
(
    NextGenFunctor * const pNextGenFunctor,
    EvoModelData   * const pEvoModelData
) :
    m_pEvoModelWork( pEvoModelData )
{
    LONG const lMaxHistSize         = Util::GetMaxNrOfSlots( EvolutionCore::GetModelSize( ) );
    LONG const lHistEntriesDemanded = Config::GetConfigValue( Config::tId::nrOfHistorySlots );
	LONG const lHistEntries         = min( lHistEntriesDemanded, lMaxHistSize * 80 / 100 );  // use only 80% of available memory
	
	assert( lHistEntries < SHRT_MAX );
    short sNrOfSlots = static_cast<short>( lHistEntries );

    HIST_GENERATION const genMaxNrOfGens = Config::GetConfigValue( Config::tId::maxGeneration );

	m_HistorySystem.InitHistorySystem
    (
        sNrOfSlots,
        genMaxNrOfGens,
        pNextGenFunctor,
        m_pEvoModelWork
    );
}

class FindGridPointFunctor : public GenerationProperty
{
public:

    FindGridPointFunctor( IndId const id ) : m_id( id ) {}

    virtual bool operator() ( HistCacheItem const * pItem ) const
    {
        EvoModelData const * pEvoModelData = static_cast< EvoModelData const * >( pItem );
        return ( pEvoModelData->FindGridPoint( m_id ).IsNotNull( ) );  // id is alive
    }

private:
    IndId const m_id;
};

HIST_GENERATION EvoHistorySys::GetFirstGenOfIndividual( IndId const & id ) const  
{ 
    return id.IsDefined( ) ? m_HistorySystem.FindFirstGenerationWithProperty( FindGridPointFunctor( id ) ) : -1; 
}

HIST_GENERATION EvoHistorySys::GetLastGenOfIndividual ( IndId const & id ) const  
{ 
    return id.IsDefined( ) ? m_HistorySystem.FindLastGenerationWithProperty ( FindGridPointFunctor( id ) ) : -1; 
}
