// NNetHistorySysGlue.cpp
//
// NNetHistGlue

#include "stdafx.h"
#include "win32_util.h"
#include "HistoryGeneration.h"
#include "HistorySystem.h"
#include "NNetHistorySysGlue.h"

class WorkThread;
class ObserverInterface;

NNetHistorySysGlue::NNetHistorySysGlue( ) :
    m_pHistorySystem( nullptr )
{}

NNetModelDataGlue * NNetHistorySysGlue::Start
(
	HistorySystem * const pHistorySystem,
	BOOL            const bAsync      
)
{
	m_pHistorySystem = pHistorySystem;

	ModelData * pModelWork = m_pHistorySystem->StartHistorySystem
    (
        100000,  //TODO
        1000,    //TODO 
		Util::GetPhysicalMemory( ),
        & m_NNetModelFactory,
		GenerationCmd::ApplicationCmd( tGenCmd::RESET, 0 )
    );

	m_HistAllocThread.Start( m_pHistorySystem, bAsync );   // delegate allocation of history slots to a work thread

	return static_cast< NNetModelDataGlue * >( pModelWork );
}

void NNetHistorySysGlue::Stop( )
{
	m_HistAllocThread.Terminate();
	m_pHistorySystem->StopHistorySystem( );
	m_pHistorySystem = nullptr;
}
