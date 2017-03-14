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
    HIST_GENERATION const genMaxNrOfGens       = Config::GetConfigValue( Config::tId::maxGeneration );
    LONG            const lHistEntriesDemanded = Config::GetConfigValue( Config::tId::nrOfHistorySlots );
    ULONGLONG       const ramBytes             = Util::GetPhysicalMemory( );                // compute number of bytes
    ULONGLONG       const ullRam               = ramBytes * 80 / 100;                       //  use only 80% of available memory
    ULONG           const ulModelSize          = EvolutionCore::GetModelSize( );
    ULONGLONG       const ullMaxHistSize       = ullRam / ulModelSize;                      assert( ullMaxHistSize < LONG_MAX );
    LONG            const lMaxHistSize         = static_cast<LONG>( ullMaxHistSize );
    LONG            const lHistEntries         = min( lHistEntriesDemanded, lMaxHistSize ); assert( lHistEntries < SHRT_MAX );
    short                 sNrOfSlots           = static_cast<short>( lHistEntries );

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

    virtual bool operator() ( BasicHistCacheItem const * pItem ) const
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
