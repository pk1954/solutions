// win32_statistics.cpp :
//
// EvoWindows

#include "stdafx.h"
#include "resource.h"
#include "Strsafe.h"
#include "XArray.h"
#include "gridRect.h"
#include "gridSelection.h"
#include "EvolutionCore.h"
#include "EvoStatistics.h"
#include "EvoReadBuffer.h"
#include "win32_baseWindow.h"
#include "win32_statistics.h"

StatisticsWindow::StatisticsWindow( )
:  TextWindow( ),
	m_pReadBuffer( nullptr ),
	m_pStatistics( nullptr )
{ 
}

StatisticsWindow::~StatisticsWindow( )
{
	delete m_pStatistics;
	m_pStatistics = nullptr;
	m_pReadBuffer = nullptr;
}

void StatisticsWindow::Start
(
    HWND            const hwndParent,
	EvoReadBuffer * const pReadBuffer
) 
{
    m_pReadBuffer = pReadBuffer;
	m_pStatistics = new EvoStatistics();
    StartTextWindow
	( 
		hwndParent, 
		PixelRect{ 200_PIXEL, 200_PIXEL, 600_PIXEL, 900_PIXEL }, 
		L"StatisticsWindow", 
		100, 
		TRUE,
		nullptr
	);
	m_pReadBuffer->RegisterObserver( this );
}

void StatisticsWindow::Stop( )
{
	Show( FALSE );
	TextWindow::StopTextWindow( );
}

void StatisticsWindow::DoPaint( TextBuffer & textBuf )
{
	EvolutionCore const * pCore = m_pReadBuffer->LockReadBuffer( );

	m_pStatistics->Prepare( * pCore, GridSelection::GetSelection(), & textBuf ); 
	if ( pCore->GetNrOfLivingIndividuals( ) == 0 )
		PostCommand2Application( IDM_STOP, 0 );

    // start printing

    textBuf.setHorizontalPos( 2_TEXT_POSITION );       
    m_pStatistics->printHeader( );  

	m_pStatistics->printCounter( L"#individuals" );  // number of individuals
    m_pStatistics->printAvAge  ( L"av. age" );       // average age
	
	textBuf.header( L"--- incidence in last generation ---" );

    m_pStatistics->printIncidence( * pCore );
	
	m_pReadBuffer->ReleaseReadBuffer( );

	textBuf.header( L"--- probabilities of actions ---" );

    m_pStatistics->printProbabilities();
	
	textBuf.header( L"--- gene statistics ---" );

    m_pStatistics->printGeneStat(  );          // percentage numbers for options

	textBuf.header( L"--- other values ---" );

	if ( EvolutionCore::IsEnabled( Action::Id::eat ) )
		m_pStatistics->printAvFood(  L"av. food" );  // average food consumption 

	m_pStatistics->printMemory( L"memory" );        // memory size counters

    // maximum memory size

    textBuf.nextLine( L"max mem" );
    textBuf.setHorizontalPos( 4_TEXT_POSITION );
    textBuf.printNumber( EvolutionCore::GetMaxPartnerMemory( ) );

	if ( EvolutionCore::IsEnabled( Action::Id::interact ) )
	{
		// nr of interactions with known culprits (Tit4Tat only)

		textBuf.nextLine( L"known" );
		textBuf.setHorizontalPos( 4_TEXT_POSITION );
		textBuf.printNumber( EvolutionCore::GetNrInteractionsWithKnownCulprit( ) );

		// nr of interactions with unknown culprits (Tit4Tat only)

		textBuf.nextLine( L"unknown" );
		textBuf.setHorizontalPos( 4_TEXT_POSITION );
		textBuf.printNumber( EvolutionCore::GetNrInteractionsWithUnknownCulprit( ) );
	}
}
