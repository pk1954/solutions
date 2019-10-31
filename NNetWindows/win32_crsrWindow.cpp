// win32_crsrWindow.cpp :
//
// NNetWindows

#include "stdafx.h"
#include <iostream>
#include <sstream> 
#include <string> 
#include <iomanip>
#include "win32_util.h"
#include "win32_textBuffer.h"
#include "win32_NNetWindow.h"
#include "win32_crsrWindow.h"

using std::wostringstream;
using std::setprecision;

CrsrWindow::CrsrWindow( ) :
	TextWindow( ),
	m_pReadBuffer( nullptr ),
	m_pNNetWindow( nullptr )
{}

CrsrWindow::~CrsrWindow( )
{
	m_pReadBuffer = nullptr;
	m_pNNetWindow = nullptr;
}

void CrsrWindow::Start
(
	HWND               const hwndParent,
	NNetReadBuffer   * const pReadBuffer,
	NNetWindow const * const pNNetWindow
) 
{
	m_pReadBuffer = pReadBuffer;
	m_pNNetWindow = pNNetWindow;
	StartTextWindow
	(
		hwndParent, 
		PixelRect { 0_PIXEL, 300_PIXEL, 300_PIXEL, 415_PIXEL }, 
		L"CrsrWindow", 
		100, 
		TRUE,
		nullptr
	);
	m_pReadBuffer->RegisterObserver( this );
}

void CrsrWindow::Stop( )
{
	TextWindow::StopTextWindow( );
	Show( FALSE );
}

void CrsrWindow::printMicroMeter
( 
	TextBuffer     & textBuf,
	MicroMeter const um 
)
{
	wostringstream wBuffer;
	wBuffer << setprecision(3) << um.GetValue() << L" um";
	textBuf.printString( wBuffer.str() );
}

void CrsrWindow::DoPaint( TextBuffer & textBuf )
{
	textBuf.printString( L"Position:" );

	PixelPoint      const pixPoint{ Util::GetRelativeCrsrPosition( m_pNNetWindow->GetWindowHandle() ) };
	MicroMeterPoint const umPoint { m_pNNetWindow->PixelPoint2MicroMeterPoint( pixPoint ) };

	printMicroMeter( textBuf, umPoint.GetX() );
	printMicroMeter( textBuf, umPoint.GetY() );
}
