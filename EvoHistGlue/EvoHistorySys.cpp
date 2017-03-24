// EvoHistorySys.cpp
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <sstream> 
#include "config.h"
#include "EvolutionCore.h"
#include "win32_util.h"
#include "HistoryGeneration.h"
#include "HistorySystem.h"
#include "EvoModelFactory.h"
#include "EvoHistorySys.h"

EvoHistorySys::EvoHistorySys
(
	EvolutionCore      * const pEvolutionCore,
	EvolutionModelData * const pEvolutionModelData
) :
    m_pEvoModelWork   ( new EvoModelData ( pEvolutionCore, pEvolutionModelData ) ),
	m_pEvoModelFactory( new EvoModelFactory( pEvolutionCore ) )
{
    LONG const lMaxHistSize         = Util::GetMaxNrOfSlots( EvolutionCore::GetModelSize( ) );
    LONG const lHistEntriesDemanded = Config::GetConfigValue( Config::tId::nrOfHistorySlots );
	LONG const lHistEntries         = min( lHistEntriesDemanded, lMaxHistSize * 80 / 100 );  // use only 80% of available memory
	
	assert( lHistEntries < SHRT_MAX );
    short sNrOfSlots = static_cast<short>( lHistEntries );

    HIST_GENERATION const genMaxNrOfGens = Config::GetConfigValue( Config::tId::maxGeneration );

    m_pHistorySystem = HistorySystem::CreateHistorySystem( );

	m_pHistorySystem->InitHistorySystem
    (
        sNrOfSlots,
        genMaxNrOfGens,
        m_pEvoModelWork,
        m_pEvoModelFactory
    );
}

EvoHistorySys::~EvoHistorySys( ) 
{
	delete m_pEvoModelFactory;
	delete m_pHistorySystem;
};

class FindGridPointFunctor : public GenerationProperty
{
public:

    FindGridPointFunctor( IndId const id ) : m_id( id ) {}

    virtual bool operator() ( ModelData const * pModelData ) const
    {
        EvoModelData const * pEvoModelData = static_cast< EvoModelData const * >( pModelData );
        return ( pEvoModelData->FindGridPoint( m_id ).IsNotNull( ) );  // id is alive
    }

private:
    IndId const m_id;
};

HIST_GENERATION EvoHistorySys::GetFirstGenOfIndividual( IndId const & id ) const  
{ 
    return id.IsDefined( ) ? m_pHistorySystem->FindFirstGenerationWithProperty( FindGridPointFunctor( id ) ) : -1; 
}

HIST_GENERATION EvoHistorySys::GetLastGenOfIndividual ( IndId const & id ) const  
{ 
    return id.IsDefined( ) ? m_pHistorySystem->FindLastGenerationWithProperty ( FindGridPointFunctor( id ) ) : -1; 
}

bool EvoHistorySys::CreateEditorCommand( tEvoCmd cmd, short sParam ) 
{ 
	if ( 
		  m_pHistorySystem->IsInHistoryMode( ) &&  // If in history mode,
		 ! askHistoryCut( m_pHistorySystem )       // all future generations will be erased
	   )
		return false;

	m_pHistorySystem->CreateAppCommand( static_cast< unsigned short >(cmd), sParam ); 
	return true;
}

bool EvoHistorySys::askHistoryCut( HistorySystem * pHistSys ) const
{
    std::wostringstream wBuffer;
    HIST_GENERATION  genCurrent  = pHistSys->GetCurrentGeneration( );
    HIST_GENERATION  genYoungest = pHistSys->GetYoungestGeneration( );
    assert( genCurrent < genYoungest );
    wBuffer << L"Gen " << ( genCurrent + 1 ) << L" - " << genYoungest << L" will be deleted.";
    return IDOK == MessageBox( nullptr, L"Cut off history?", wBuffer.str( ).c_str( ), MB_OKCANCEL | MB_SYSTEMMODAL );
}

