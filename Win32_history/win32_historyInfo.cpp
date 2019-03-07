// win32_historyInfo.cpp :
//

#include "stdafx.h"
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
    StartTextWindow
	( 
		hwndParent, 
		PixelRect { 0_PIXEL_X, 300_PIXEL_Y, 300_PIXEL_X, 415_PIXEL_Y }, 
		L"HistInfoWindow", 
		100, 
		TRUE 
	);
}

void HistInfoWindow::DoPaint( TextBuffer & textBuf )
{
	int iNrOfUsedSlots { m_pHistSystem->GetNrOfUsedHistCacheSlots( ).GetValue() };
    int iNrOfSlots     { m_pHistSystem->GetNrOfHistCacheSlots( ).GetValue() };
    BYTES slotSize     { m_pHistSystem->GetSlotSize( ) };
	BYTES totalSize    { slotSize.GetValue() * iNrOfSlots };

	textBuf.printString( L"used slots  " );
	textBuf.setHorizontalPos( 3_TEXT_POSITION );
	textBuf.printPercentage( iNrOfUsedSlots, iNrOfSlots );

    textBuf.nextLine( L"slot size   " );
	textBuf.setHorizontalPos( 3_TEXT_POSITION );
	textBuf.printNumber( slotSize.GetValue() );

    textBuf.nextLine( L"cache size  " );
	textBuf.setHorizontalPos( 3_TEXT_POSITION );
	textBuf.printNumber( totalSize.GetValue() );

    textBuf.nextLine( L"genCurrent  " );
	textBuf.setHorizontalPos( 3_TEXT_POSITION );
	textBuf.printNumber( m_pHistSystem->GetCurrentGeneration( ).GetLong() );
}
