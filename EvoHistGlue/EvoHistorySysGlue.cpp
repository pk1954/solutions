// EvoHistorySysGlue.cpp
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <sstream> 
#include <limits.h>
#include "config.h"
#include "win32_rootWindow.h"
#include "HistAllocThread.h"
#include "HistoryGeneration.h"
#include "HistorySystem.h"
#include "EvoModelFactory.h"
#include "EvolutionTypes.h"
#include "EvoHistorySysGlue.h"

class WorkThread;
class ObserverInterface;

GenerationCmd const EvoHistorySysGlue::NEXT_GEN_CMD = EvoHistorySysGlue::EvoCmd( tEvoCmd::nextGen, 0 );

EvoHistorySysGlue::EvoHistorySysGlue( ) :
    m_pEvoModelWork   ( nullptr ),
	m_pEvoModelFactory( nullptr ),
    m_pHistorySystem  ( nullptr ),
	m_pHistAllocThread( nullptr ),
	m_bAskHistoryCut  ( false )
{}

HistSlotNr EvoHistorySysGlue::calcHistCacheSize()
{
	BYTES      const slotSize             { m_pHistorySystem->GetSlotSize() };
	long       const lMaxHistSize         { Util::GetMaxNrOfSlots( slotSize ) };
	long       const lHistEntriesDemanded { Config::GetConfigValue( Config::tId::nrOfHistorySlots ) };
	long       const lHistEntries         { min( lHistEntriesDemanded, lMaxHistSize * 70 / 100 ) };  // use only 80% of available memory
	HistSlotNr const nrOfSlots            { CastToShort( lHistEntries ) }; 
	return nrOfSlots;
}

void EvoHistorySysGlue::Start
(
	EvolutionCore * const pCore, 
	HistorySystem * const pHistorySystem,
	bool            const bAskHistoryCut, // true: ask user for history cut, false: cut without asking
	RootWindow    * const pRootWindow
)
{
	m_pHistorySystem   = pHistorySystem;
	m_bAskHistoryCut   = bAskHistoryCut;
    m_pEvoModelWork    = new EvoModelDataGlue( pCore );
	m_pEvoModelFactory = new EvoModelFactory ( );

	pHistorySystem->InitHistorySystem
    (
        Config::GetConfigValue( Config::tId::maxGeneration ),
        m_pEvoModelWork
    );

	pHistorySystem->StartHistorySystem
    (
        calcHistCacheSize(), // needs m_pEvoModelWork in HistorySystem!
        m_pEvoModelFactory,
		pRootWindow,
		GenerationCmd::ApplicationCmd( static_cast< tGenCmd >( tEvoCmd::reset ), 0 )
    );

	m_pHistAllocThread = new HistAllocThread( m_pHistorySystem, TRUE );   // delegate allocation of history slots to a work thread
}

EvoHistorySysGlue::~EvoHistorySysGlue( ) 
{
	m_pHistAllocThread->Terminate();
	m_pHistorySystem->ShutDownHistCache();
	delete m_pHistAllocThread;
	delete m_pEvoModelFactory;
	delete m_pEvoModelWork;
};

class FindGridPointFunctor : public GenerationProperty
{
public:

    FindGridPointFunctor( IND_ID const id ) : m_id( id ) {}

    virtual bool operator() ( ModelData const * pModelData ) const
    {
        EvoModelDataGlue const * pEvoModelData = static_cast< EvoModelDataGlue const * >( pModelData );
        return ( pEvoModelData->FindGridPoint( m_id ).IsNotNull( ) );  // id is alive
    }

private:
    IND_ID const m_id;
};

HIST_GENERATION EvoHistorySysGlue::GetGenWithIndividual( IND_ID const & id, bool const bReverse ) const  
{ 
	return id.IsNull( ) 
		   ? HIST_GENERATION()
           : m_pHistorySystem->FindGenerationWithProperty( FindGridPointFunctor( id ), bReverse );
}

void EvoHistorySysGlue::EvoClearHistory(  ) 
{ 
    m_pHistorySystem->ApproachHistGen( 0 );
	assert( m_pHistorySystem->GetCurrentGeneration( ) == 0 );
	m_pHistorySystem->ClearHistory( 0 );
}

bool EvoHistorySysGlue::EvoCreateEditorCommand( GenerationCmd cmd ) 
{ 
	if ( 
		  m_pHistorySystem->IsInHistoryMode( ) &&  // If in history mode,
          m_bAskHistoryCut &&                      // and asking user is turned on
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
