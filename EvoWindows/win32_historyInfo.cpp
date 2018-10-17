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
    HWND          const   hwndParent,
	HistorySystem const * pHistSys
) 
{
    StartTextWindow( hwndParent, 0, 300, 300, 150, L"HistInfoWindow", 100, FALSE );
//    Move( 0, 300, 300, 150, TRUE );
	m_pHistSystem = pHistSys;
}

void HistInfoWindow::DoPaint( TextBuffer & textBuf )
{
    int iNrOfUsedSlots   = m_pHistSystem->GetNrOfUsedHistCacheSlots( );
    int iNrOfSlots       = m_pHistSystem->GetNrOfHistCacheSlots( );
    long long lSlotSize  = m_pHistSystem->GetSlotSize( );
    long long lTotalSize = iNrOfSlots * lSlotSize;

	textBuf.printString( L"used slots  " );
	textBuf.setHorizontalPos( 3 );
	textBuf.printPercentage( iNrOfUsedSlots, iNrOfSlots );

    textBuf.nextLine( L"slot size   " );
	textBuf.printNumber( lSlotSize );

    textBuf.nextLine( L"cache size  " );
	textBuf.printNumber( lTotalSize );

    textBuf.nextLine( L"genCurrent  " );
	textBuf.setHorizontalPos( 3 );
	textBuf.printNumber( m_pHistSystem->GetCurrentGeneration( ).GetLong() );
}
