// win32_performanceWindow.cpp :
//

#include "stdafx.h"
#include "Strsafe.h"
#include <chrono>
#include "util.h"
#include "win32_actionTimer.h"
#include "win32_performanceWindow.h"

using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::chrono::duration_cast;

PerformanceWindow::PerformanceWindow( ) : 
    TextWindow( ),
	m_pDelay( nullptr ),
	m_pAtComputation( nullptr ),
	m_pAtDisplay( nullptr )
{ 
}

void PerformanceWindow::Start
( 
	HWND                  const hwndParent, 
	Delay                     & delay,
	ActionTimer               & atCompute, 
	ActionTimer               & atDisplay,
	std::function<bool()> const visibilityCriterion 
)
{
	m_pDelay         = & delay;
	m_pAtComputation = & atCompute;
	m_pAtDisplay     = & atDisplay;
	StartTextWindow
	( 
		hwndParent, 
		PixelRect { 0_PIXEL, 0_PIXEL, 300_PIXEL, 130_PIXEL }, 
		L"PerformanceWindow", 
		100,  // alpha
		TRUE,
		visibilityCriterion
	);
	m_pAtComputation->RegisterObserver( this );  // notify me on computation performance data changes 
	m_pAtDisplay->RegisterObserver( this );      // notify me on display performance data changes 
	m_pDelay->RegisterObserver( this );          // notify me if delay changes
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
	microseconds    const duration
)
{
	textBuf.printString       ( pwch1 );
	textBuf.printAsDecFraction( CastToUnsignedLong(duration.count()) );
	textBuf.printString       ( L"ms" );
	textBuf.nextLine          ( );
}

void PerformanceWindow::printLine
(
	TextBuffer          & textBuf,
	wchar_t const * const pwch1, 
	MilliHertz      const frequency
)
{
	textBuf.printString       ( pwch1 );
	textBuf.printAsDecFraction( CastToUnsignedLong(frequency.GetValue()) );
	textBuf.printString       ( L"Gen/s" );
	textBuf.nextLine          ( );
}

void PerformanceWindow::DoPaint( TextBuffer & textBuf )
{      
	milliseconds const msDelay            = m_pDelay->GetDelay();
	microseconds const usDelay            = duration_cast<microseconds>(msDelay);
	microseconds const usModelTime        = m_pAtComputation->GetSingleActionTime( );
	microseconds const usSum              = usModelTime + usDelay;
	MilliHertz   const milliHertzComputed = m_pAtComputation->CalcActionFrequency( usSum );
	MilliHertz   const miliHertzMeasured  = m_pAtComputation->GetMeasuredPerformance( );
	microseconds const usDisplayTime      = m_pAtDisplay->GetSingleActionTime( );

    printLine( textBuf, L"Delay:  ", usDelay );
    printLine( textBuf, L"Model:  ", usModelTime );
    printLine( textBuf, L"Comp:   ", milliHertzComputed );
    printLine( textBuf, L"Meas:   ", miliHertzMeasured );
    printLine( textBuf, L"Display:", usDisplayTime );
}
