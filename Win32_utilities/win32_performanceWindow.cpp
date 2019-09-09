// win32_performanceWindow.cpp :
//

#include "stdafx.h"
#include "Strsafe.h"
#include "win32_actionTimer.h"
#include "win32_performanceWindow.h"

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
	DWORD           const dwValue, 
	wchar_t const * const pwch2 
)
{
    textBuf.printString       ( pwch1 );
    textBuf.printAsDecFraction( dwValue );
    textBuf.printString       ( pwch2 );
    textBuf.nextLine          ( );
}

void PerformanceWindow::DoPaint( TextBuffer & textBuf )
{
	DWORD const dwDelay              = m_pDelay->GetDelay();
	DWORD const dwModelTime          = m_pAtComputation->GetSingleActionTime();
	DWORD const dwSumInMicroSeconds  = dwModelTime + dwDelay * 1000;
    DWORD const dwGensPer1000SecComp = m_pAtComputation->CalcFrequency( 1000, dwSumInMicroSeconds );
    DWORD const dwGensPer1000SecMeas = m_pAtComputation->GetMeasuredPerformance( );
	DWORD const dwDisplayTime        = m_pAtDisplay->GetSingleActionTime( );

    printLine( textBuf, L"Model:  ", dwModelTime,          L"ms"    );
    printLine( textBuf, L"Delay:  ", dwDelay * 1000,       L"ms"    );
    printLine( textBuf, L"Comp:   ", dwGensPer1000SecComp, L"Gen/s" );
    printLine( textBuf, L"Meas:   ", dwGensPer1000SecMeas, L"Gen/s" );
    printLine( textBuf, L"Display:", dwDisplayTime,        L"ms"    );
}
