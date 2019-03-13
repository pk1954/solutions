// win32_statistics.cpp :
//
// EvoWindows

#include "stdafx.h"
#include "Strsafe.h"
#include "XArray.h"
#include "gridRect.h"
#include "EvolutionCore.h"
#include "EvoStatistics.h"
#include "win32_baseWindow.h"
#include "win32_statistics.h"
#include "win32_stopwatch.h"

StatisticsWindow::StatisticsWindow( ):
    TextWindow( )
{ }

void StatisticsWindow::Start
(
    HWND                  const hwndParent,
    EvolutionCore const * const pCore
) 
{
    m_pCore = pCore;
	m_pStatistics = new EvoStatistics( m_pCore );
    StartTextWindow
	( 
		hwndParent, 
		PixelRect{ 200_PIXEL_X, 200_PIXEL_Y, 600_PIXEL_X, 900_PIXEL_Y }, 
		L"StatisticsWindow", 
		100, 
		TRUE 
	);
}

StatisticsWindow::~StatisticsWindow( )
{
	delete m_pStatistics;
	m_pStatistics = nullptr;
	m_pCore = nullptr;
}

Stopwatch stopwatch;

void StatisticsWindow::DoPaint( TextBuffer & textBuf )
{
//	stopwatch.Start();
 
	m_pStatistics->Initialize( & textBuf );   // aquire and prepare data 

    // start printing

    textBuf.setHorizontalPos( 2_TEXT_POSITION );       
    m_pStatistics->printHeader  ( );  

	m_pStatistics->printCounter ( L"#individuals" );  // number of individuals
    m_pStatistics->printAvAge   ( L"av. age" );       // average age
	
	textBuf.nextLine( L"---" );

    m_pStatistics->printIncidence( );
	
	textBuf.nextLine( L"---" );

    m_pStatistics->printProbabilities();
	
	textBuf.nextLine( L"---" );

    m_pStatistics->printGeneStat(  );          // percentage numbers for options

	textBuf.nextLine( L"---" );

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
//	stopwatch.Stop( L"Statistics" );
}
