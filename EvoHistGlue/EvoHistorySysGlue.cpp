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
	m_pEvoModelFactory( nullptr ),
    m_pHistorySystem  ( nullptr ),
	m_pHistAllocThread( nullptr ),
	m_bAskHistoryCut  ( false )
{}

EvoModelDataGlue * EvoHistorySysGlue::Start
(
	HistorySystem     * const pHistorySystem,
	bool                const bAskHistoryCut, // true: ask user for history cut, false: cut without asking
	ObserverInterface * const pObserver
)
{
	m_pHistorySystem   = pHistorySystem;
	m_bAskHistoryCut   = bAskHistoryCut;
	m_pEvoModelFactory = new EvoModelFactory( ); //ok

	ModelData * pModelWork = m_pHistorySystem->StartHistorySystem
    (
        Config::GetConfigValue( Config::tId::maxGeneration ),
        Config::GetConfigValue( Config::tId::nrOfHistorySlots ),
		Util::GetPhysicalMemory( ),
        m_pEvoModelFactory,
		pObserver,
		GenerationCmd::ApplicationCmd( static_cast< tGenCmd >( tEvoCmd::reset ), 0 )
    );

	// delegate allocation of history slots to a work thread

	m_pHistAllocThread = new HistAllocThread( m_pHistorySystem, TRUE );  //ok 

	return static_cast< EvoModelDataGlue * >( pModelWork );
}

void EvoHistorySysGlue::Stop( )
{
	m_pHistAllocThread->Terminate();
	m_pHistorySystem->StopHistorySystem( );

	delete m_pEvoModelFactory;  //ok
	delete m_pHistAllocThread;  //ok

	m_pEvoModelFactory = nullptr;
	m_pHistorySystem   = nullptr;
	m_pHistAllocThread = nullptr;
}

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
