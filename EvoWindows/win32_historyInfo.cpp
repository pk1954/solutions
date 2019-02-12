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
	m_pHistSystem = pHistSys;
    StartTextWindow( hwndParent, 0_PIXEL, 300_PIXEL, 300_PIXEL, 115_PIXEL, L"HistInfoWindow", 100, TRUE );
}

void HistInfoWindow::DoPaint( TextBuffer & textBuf )
{
    int iNrOfUsedSlots   = m_pHistSystem->GetNrOfUsedHistCacheSlots( );
    int iNrOfSlots       = m_pHistSystem->GetNrOfHistCacheSlots( );
    long long lSlotSize  = m_pHistSystem->GetSlotSize( );
    long long lTotalSize = iNrOfSlots * lSlotSize;

	textBuf.printString( L"used slots  " );
	textBuf.setHorizontalPos( 3_TEXT_POSITION );
	textBuf.printPercentage( iNrOfUsedSlots, iNrOfSlots );

    textBuf.nextLine( L"slot size   " );
	textBuf.printNumber( lSlotSize );

    textBuf.nextLine( L"cache size  " );
	textBuf.printNumber( lTotalSize );

    textBuf.nextLine( L"genCurrent  " );
	textBuf.setHorizontalPos( 3_TEXT_POSITION );
	textBuf.printNumber( m_pHistSystem->GetCurrentGeneration( ).GetLong() );
}
