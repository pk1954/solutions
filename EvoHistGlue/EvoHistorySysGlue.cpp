// EvoHistorySysGlue.cpp
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <sstream> 
#include <limits.h>
#include "config.h"
#include "HistAllocThread.h"
#include "HistoryGeneration.h"
#include "HistorySystem.h"
#include "EvoModelFactory.h"
#include "EvoHistorySysGlue.h"

class WorkThread;

GenerationCmd const EvoHistorySysGlue::NEXT_GEN_CMD = EvoHistorySysGlue::EvoCmd( tEvoCmd::nextGen, 0 );

EvoHistorySysGlue::EvoHistorySysGlue( ) :
    m_pEvoModelWork   ( nullptr ),
	m_pEvoModelFactory( nullptr ),
    m_pHistorySystem  ( nullptr ),
	m_pHistAllocThread( nullptr ),
	m_bAskHistoryCut  ( false )
{}

void EvoHistorySysGlue::Start
(
	EvolutionModelData * const pEvolutionModelData,
	EvolutionCore      * const pCore, 
	long                 const lMaxHistSize,
	bool                 const bAskHistoryCut // true: ask user for history cut, false: cut without asking
)
{
    long const lHistEntriesDemanded = Config::GetConfigValue( Config::tId::nrOfHistorySlots );
	long const lHistEntries         = min( lHistEntriesDemanded, lMaxHistSize * 80 / 100 );  // use only 80% of available memory
	
    short sNrOfSlots = CastToShort( lHistEntries );

    HIST_GENERATION const genMaxNrOfGens = Config::GetConfigValue( Config::tId::maxGeneration );

    m_pEvoModelWork    = new EvoModelDataGlue ( pEvolutionModelData, pCore );
	m_pEvoModelFactory = new EvoModelFactory( pCore );

    m_pHistorySystem = HistorySystem::CreateHistorySystem( );

	m_pHistorySystem->InitHistorySystem
    (
        sNrOfSlots,
        genMaxNrOfGens,
        m_pEvoModelWork,
        m_pEvoModelFactory,
		GenerationCmd::ApplicationCmd( static_cast< tGenCmd >( tEvoCmd::reset ), 0 )
    );

	m_bAskHistoryCut = bAskHistoryCut;

	m_pHistAllocThread = new HistAllocThread( m_pHistorySystem, TRUE );   // delegate allocation of history slots to a work thread
}

EvoHistorySysGlue::~EvoHistorySysGlue( ) 
{
    shutDownHistoryCache( );
	delete m_pHistAllocThread;
	delete m_pEvoModelFactory;
	delete m_pHistorySystem;
};

class FindGridPointFunctor : public GenerationProperty
{
public:

    FindGridPointFunctor( IndId const id ) : m_id( id ) {}

    virtual bool operator() ( ModelData const * pModelData ) const
    {
        EvoModelDataGlue const * pEvoModelData = static_cast< EvoModelDataGlue const * >( pModelData );
        return ( pEvoModelData->FindGridPoint( m_id ).IsNotNull( ) );  // id is alive
    }

private:
    IndId const m_id;
};

HIST_GENERATION EvoHistorySysGlue::GetFirstGenOfIndividual( IndId const & id ) const  
{ 
	return id.IsDefined( ) ? m_pHistorySystem->FindFirstGenerationWithProperty( FindGridPointFunctor( id ) ) : -1; 
}

HIST_GENERATION EvoHistorySysGlue::GetLastGenOfIndividual ( IndId const & id ) const  
{ 
	return id.IsDefined( ) ? m_pHistorySystem->FindLastGenerationWithProperty( FindGridPointFunctor( id ) ) : -1; 
}

bool EvoHistorySysGlue::EvoCreateEditorCommand( GenerationCmd cmd ) 
{ 
	if ( 
		  m_pHistorySystem->IsInHistoryMode( ) &&  // If in history mode,
          m_bAskHistoryCut &&                      // and asking user is requested
	      ! askHistoryCut( m_pHistorySystem )      // ask user, if all future generations should be erased
	   )
		return false;  // user answered no, do not erase

	m_pHistorySystem->ClearHistory( GetCurrentGeneration( ) );  // if in history mode: cut off future generations
	m_pHistorySystem->CreateAppCommand( cmd ); 
	return true;
}

bool EvoHistorySysGlue::askHistoryCut( HistorySystem * pHistSys ) const
{
    std::wostringstream wBuffer;
    HIST_GENERATION  genCurrent  = pHistSys->GetCurrentGeneration( );
    HIST_GENERATION  genYoungest = pHistSys->GetYoungestGeneration( );
    assert( genCurrent < genYoungest );
    wBuffer << L"Gen " << ( genCurrent + 1 ) << L" - " << genYoungest << L" will be deleted.";
    return IDOK == MessageBox( nullptr, L"Cut off history?", wBuffer.str( ).c_str( ), MB_OKCANCEL | MB_SYSTEMMODAL );
}

void EvoHistorySysGlue::shutDownHistoryCache( )
{
    int iMax = GetNrOfHistCacheSlots( ) - 1;
//    int iPercentLast = 0;
    for ( int iRun = iMax; iRun >= 0; --iRun )
    {
/*
        int iPercent = ( iRun * 100 ) / iMax;
        if ( iPercent != iPercentLast )
        {
            std::wstring wstrLine = L"... deleting history buffer: " + to_wstring( iPercent ) + L"%";
            m_pStatusBar->DisplayStatusLine( wstrLine );
            iPercentLast = iPercent;
        }
*/
        m_pHistorySystem->ShutDownHistCacheSlot( iRun );
    }
}
