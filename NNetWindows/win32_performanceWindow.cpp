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
	textBuf.printString( pwch1 );
	textBuf.printNumber( CastToFloat( dData * 100.0f ) );
	textBuf.nextLine   ( );
}

void PerformanceWindow::DoPaint( TextBuffer & textBuf )
{      
//	microseconds const usModelTime   = m_pAtComputation->GetSingleActionTime( );
//	Hertz        const HertzComputed = m_pAtComputation->CalcActionFrequency( usSum );
//	Hertz        const HertzMeasured = m_pAtComputation->GetMeasuredPerformance( );

//    printLine( textBuf, L"Model:  ", usModelTime );
//    printLine( textBuf, L"Comp:   ", HertzComputed );
//    printLine( textBuf, L"Meas:   ", HertzMeasured );

	printLine( textBuf, L"Duty cycle: ", m_pNNetWorkThreadInterface->GetDutyCycle( ) );

}
