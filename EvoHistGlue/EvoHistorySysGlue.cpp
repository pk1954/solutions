// EvoHistorySysGlue.cpp
//
// EvoHistGlue

#include "stdafx.h"
#include "config.h"
#include "win32_util.h"
#include "HistoryGeneration.h"
#include "HistorySystem.h"
#include "EvolutionTypes.h"
#include "EvoHistorySysGlue.h"

class WorkThread;

EvoHistorySysGlue::EvoHistorySysGlue( ) :
    m_pHistorySystem( nullptr )
{}

EvoModelDataGlue * EvoHistorySysGlue::Start
(
	HistorySystem * const pHistorySystem,
	BOOL            const bAsync      
)
{
	m_pHistorySystem = pHistorySystem;

	ModelData * pModelWork = m_pHistorySystem->StartHistorySystem
    (
        Config::GetConfigValue( Config::tId::maxGeneration ),
        Config::GetConfigValue( Config::tId::nrOfHistorySlots ),
		Util::GetPhysicalMemory( ),
        & m_EvoModelFactory,
		GenerationCmd::ApplicationCmd( tGenCmd::RESET, 0 )
    );

	m_HistAllocThread.Start( m_pHistorySystem, bAsync );   // delegate allocation of history slots to a work thread

	return static_cast< EvoModelDataGlue * >( pModelWork );
}

void EvoHistorySysGlue::Stop( )
{
	m_HistAllocThread.Terminate();
	m_pHistorySystem->StopHistorySystem( );
	m_pHistorySystem = nullptr;
}

class FindGridPointFunctor : public GenerationProperty
{
public:

    FindGridPointFunctor( IND_ID const id ) : m_id( id ) {}

    virtual bool operator() ( ModelData const * pModelData ) const
    {
        EvoModelDataGlue const * pEvoModelData = static_cast< EvoModelDataGlue const * >( pModelData );
        return ( pEvoModelData->FindGridPointFromId( m_id ).IsNotNull( ) );  // id is alive
    }

private:
    IND_ID const m_id;
};

HIST_GENERATION EvoHistorySysGlue::GetGenWithIndividual( GridPoint const gp, bool const bReverse ) const
{
	HIST_GENERATION       gen   = m_pHistorySystem->GetCurrentGeneration( );
	EvolutionCore const * pCore = getEvolutionCore( gen );
	IND_ID                id    = pCore->GetId( gp );
	return id.IsNull( ) 
		? HIST_GENERATION()
		: m_pHistorySystem->FindGenerationWithProperty( FindGridPointFunctor( id ), bReverse );
}

EvolutionCore const * EvoHistorySysGlue::getEvolutionCore( HIST_GENERATION const gen ) const
{
	ModelData const * pModelData { m_pHistorySystem->GetModelData( gen ) };
	return ( pModelData == nullptr )
	? nullptr  // gen not in cache
	: (static_cast< EvoModelDataGlue const * >( pModelData ))->GetEvolutionCoreC( );
}
