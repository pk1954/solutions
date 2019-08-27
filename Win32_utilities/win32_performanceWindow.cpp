// win32_performanceWindow.cpp :
//

#include "stdafx.h"
#include "Strsafe.h"
#include "win32_performanceWindow.h"

PerformanceWindow::PerformanceWindow( ) : 
    TextWindow( ),
	m_pDelay( nullptr ),
    m_hTimer( nullptr ),
    m_hrtimDisplay( ),
    m_dwDisplayTime( 0 )
{ 
}

void PerformanceWindow::Start
( 
	HWND  const hwndParent, 
	Delay     & delay,
	std::function<bool()> const visibilityCriterion 
)
{
	m_pDelay = & delay;
    StartTextWindow
	( 
		hwndParent, 
		PixelRect { 0_PIXEL, 0_PIXEL, 300_PIXEL, 130_PIXEL }, 
		L"PerformanceWindow", 
		100,  // alpha
		TRUE,
		visibilityCriterion
	);
	m_pDelay->RegisterPerformanceObserver( this ); // notify me on perormance data changes 
	m_pDelay->RegisterDelayObserver( this );       // notify me if delay changes
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
	DWORD const dwModelTime          = m_pDelay->GetModelTime();
	DWORD const dwSumInMicroSeconds  = dwModelTime + dwDelay * 1000;
    DWORD const dwGensPer1000SecComp = m_pDelay->CalcFrequency( 1000, dwSumInMicroSeconds );
    DWORD const dwGensPer1000SecMeas = m_pDelay->GetMeasuredPerformance( );

    printLine( textBuf, L"Model:  ", dwModelTime,          L"ms"    );
    printLine( textBuf, L"Delay:  ", dwDelay * 1000,       L"ms"    );
    printLine( textBuf, L"Comp:   ", dwGensPer1000SecComp, L"Gen/s" );
    printLine( textBuf, L"Meas:   ", dwGensPer1000SecMeas, L"Gen/s" );
    printLine( textBuf, L"Display:", m_dwDisplayTime,      L"ms"    );
}
