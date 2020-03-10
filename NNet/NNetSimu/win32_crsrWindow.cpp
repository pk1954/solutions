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
using std::endl;

CrsrWindow::CrsrWindow( ) :
	TextWindow( )
{}

CrsrWindow::~CrsrWindow( )
{
	m_pNNetWindow = nullptr;
}

void CrsrWindow::Start
(
	HWND               const hwndParent,
	Observable       * const pObservable,
	NNetWindow const * const pNNetWindow
) 
{
	m_pNNetWindow = pNNetWindow;
	StartTextWindow
	(
		hwndParent, 
		PixelRect { 0_PIXEL, 300_PIXEL, 300_PIXEL, 500_PIXEL }, 
		L"CrsrWindow", 
		100, 
		TRUE,
		nullptr
	);
	pObservable->RegisterObserver( this );
//	::CreateWindowToolTip( GetWindowHandle(), L"Cursor window" );
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
	MicroMeter     umAbs { abs( um.GetValue() ) };
	wBuffer << std::fixed << setprecision(0);
	if ( umAbs < 1000.0_MicroMeter )
		wBuffer << um.GetValue() << L" um";
	else if ( umAbs < 1000000.0_MicroMeter )
		wBuffer << um.GetValue() / 1000.0f << L" mm";
	else
		wBuffer << um.GetValue() / 1000000.0f << L" m";
	textBuf.printString( wBuffer.str() );
}

void CrsrWindow::DoPaint( TextBuffer & textBuf )
{
	HWND       const hwnd    { m_pNNetWindow->GetWindowHandle() };
	PixelPoint const pixPoint{ Util::GetRelativeCrsrPosition( hwnd ) };
	if ( Util::IsInClientRect( hwnd, pixPoint ) )
	{
		MicroMeterPoint const umPoint { m_pNNetWindow->PixelPoint2MicroMeterPoint( pixPoint ) };
		textBuf.printString( L"Position:" );
		printMicroMeter( textBuf, umPoint.GetX() );
		printMicroMeter( textBuf, umPoint.GetY() );
	}
	else
	{
		textBuf.AlignLeft();
		textBuf.printString( L"Cursor not in model window" );
	}
}
