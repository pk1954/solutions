// win32_historyInfo.cpp :
//

#include "stdafx.h"
#include "resource.h"
#include <fstream>
#include <iostream>
#include <sstream> 
#include <locale>
#include "observerInterface.h"
#include "HistoryGeneration.h"
#include "HistorySystem.h"
#include "win32_historyInfo.h"

HistInfoWindow::HistInfoWindow( ) :
    TextWindow( ),
    m_pHistSystem( nullptr )
{ }

void HistInfoWindow::Start
(
    HWND              const   hWndParent,
	HistorySystem     const * pHistSys
) 
{
    HWND hWnd = StartTextWindow( hWndParent, L"HistInfoWindow", 100 );
    Move( 0, 300, 300, 150, TRUE );
	m_pHistSystem = pHistSys;
}

void HistInfoWindow::DoPaint( )
{
    long long lNrOfSlots = m_pHistSystem->GetNrOfHistCacheSlots( );
    long long lSlotSize  = m_pHistSystem->GetSlotSize( );
    long long lTotalSize = lNrOfSlots * lSlotSize;

	printString( L"# of slots  " );
	printNumber( lNrOfSlots );

    nextLine( L"slot size   " );
	printNumber( lSlotSize );

    nextLine( L"total size  " );
	printNumber( lTotalSize );

    nextLine( L"genCurrent  " );
	setHorizontalPos( 3 );
	printNumber( m_pHistSystem->GetCurrentGeneration( ).GetLong() );

	nextLine( L"genYoungest " );
	setHorizontalPos( 3 );
	printNumber( m_pHistSystem->GetYoungestGeneration( ).GetLong() );
}
