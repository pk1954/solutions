// win32_performanceWindow.cpp :
//
// EvoWindows

#include "stdafx.h"
#include "Strsafe.h"
#include <chrono>
#include "util.h"
#include "win32_NNetWorkThreadInterface.h"
#include "win32_performanceWindow.h"

PerformanceWindow::PerformanceWindow( ) : 
    TextWindow( ),
	m_pNNetWorkThreadInterface( nullptr )
{ 
}

void PerformanceWindow::Start
( 
	HWND                      const hwndParent,
	NNetWorkThreadInterface * const pNNetWorkThreadInterface
)
{
	StartTextWindow
	( 
		hwndParent, 
		PixelRect { 0_PIXEL, 0_PIXEL, 300_PIXEL, 130_PIXEL }, 
		L"PerformanceWindow", 
		100,  // alpha
		TRUE,
		nullptr
	);
	m_pNNetWorkThreadInterface = pNNetWorkThreadInterface;
	m_pNNetWorkThreadInterface->AddPerformanceObserver( this ); // notify me on computation performance changes 
}

void PerformanceWindow::Stop( )
{
	TextWindow::StopTextWindow( );
	Show( FALSE );
}

void PerformanceWindow::printLine
(
	TextBuffer          & textBuf,
	wchar_t const * const pwch1, 
	double          const dData
)
{
	std::wostringstream wBuffer;
	float fPrintValue { CastToFloat( dData * 100.0f ) };
	wBuffer << std::fixed << std::setprecision(3) << fPrintValue << L'%';
	textBuf.printString( pwch1 );
	textBuf.printString( L"" );
	textBuf.printString( wBuffer.str() );
	textBuf.nextLine   ( );
}

void PerformanceWindow::DoPaint( TextBuffer & textBuf )
{      
	printLine( textBuf, L"Duty cycle: ", m_pNNetWorkThreadInterface->GetDutyCycle( ) );
}
