// win32_performanceWindow.cpp :
//

#include "stdafx.h"
#include "Strsafe.h"
#include "win32_performanceWindow.h"

using namespace std;

PerformanceWindow::PerformanceWindow( ) : 
    TextWindow( ),
    m_hTimer( nullptr ),
    m_hrtimOverall( ),
    m_hrtimDisplay( ),
    m_hrtimCompute( ),
    m_dwModelTime( 0 ),
    m_dwDisplayTime( 0 ),
    m_dwGenerationDelay( 0 ),
    m_dwCounter( 0 )
{ }

void PerformanceWindow::Start( HWND const hwndParent, UINT const uiAlpha )
{
    StartTextWindow( hwndParent, 0, 0, 300, 130, L"PerformanceWindow", uiAlpha, TRUE );
}

DWORD PerformanceWindow::getMeasuredPerformance( )
{
    DWORD dwResult;
    DWORD dwMicroSecs;

    m_hrtimOverall.Stop( );
    dwMicroSecs = m_hrtimOverall.Get( );
    m_hrtimOverall.Start( );
    dwResult = dwMicroSecs ? ( ( m_dwCounter * 1000 * 1000 ) / dwMicroSecs ) * 1000 : 0;
    m_dwCounter = 0;

    return dwResult;
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
    DWORD const dwSumInMicroSeconds  = m_dwModelTime + m_dwGenerationDelay * 1000;
    DWORD const dwGensPer1000SecComp = dwSumInMicroSeconds ? ( 1000 * 1000 * 1000 / dwSumInMicroSeconds ) : 0;
    DWORD const dwGensPer1000SecMeas = getMeasuredPerformance( );

    printLine( textBuf, L"Model:  ", m_dwModelTime,              L"ms"    );
    printLine( textBuf, L"Delay:  ", m_dwGenerationDelay * 1000, L"ms"    );
    printLine( textBuf, L"Comp:   ", dwGensPer1000SecComp,       L"Gen/s" );
    printLine( textBuf, L"Meas:   ", dwGensPer1000SecMeas,       L"Gen/s" );
    printLine( textBuf, L"Display:", m_dwDisplayTime,            L"ms"    );
}
