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
    m_pHistSys( nullptr )
{ }

void HistInfoWindow::Start( HWND const hwndParent, std::function<bool()> const visibilityCriterion ) 
{
	StartTextWindow
	( 
		hwndParent, 
		PixelRect { 0_PIXEL, 300_PIXEL, 300_PIXEL, 415_PIXEL }, 
		L"HistInfoWindow", 
		100, 
		TRUE,
		visibilityCriterion
	);
}

void HistInfoWindow::Stop( ) 
{
	TextWindow::StopTextWindow( );
	m_pHistSys->UnregisterAllObservers();
	m_pHistSys = nullptr;
}

void HistInfoWindow::SetHistorySystem( HistorySystem * pHistSys ) 
{
	m_pHistSys = pHistSys;
	m_pHistSys->RegisterObserver( this );  // Trigger me, if something happens in history system
}

void HistInfoWindow::DoPaint( TextBuffer & textBuf )
{
	if ( m_pHistSys )
	{
		int iNrOfUsedSlots { m_pHistSys->GetNrOfUsedHistCacheSlots( ).GetValue() };
		int iNrOfSlots     { m_pHistSys->GetNrOfHistCacheSlots( ).GetValue() };
		BYTES slotSize     { m_pHistSys->GetSlotSize( ) };
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
		textBuf.printNumber( m_pHistSys->GetCurrentGeneration( ).GetLong() );
	}
}
