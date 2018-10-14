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
    StartTextWindow( hwndParent, L"PerformanceWindow", uiAlpha, FALSE );
	Move( 0, 0, 300, 130, TRUE );  // set reasonable window size
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

void PerformanceWindow::printLine( wchar_t const * const pwch1, DWORD const dwValue, wchar_t const * const pwch2 )
{
    printString    ( pwch1 );
    printAsDecValue( dwValue );
    printString    ( pwch2 );
    nextLine( );
}

void PerformanceWindow::DoPaint( )
{
    DWORD const dwSumInMicroSeconds  = m_dwModelTime + m_dwGenerationDelay * 1000;
    DWORD const dwGensPer1000SecComp = dwSumInMicroSeconds ? ( 1000 * 1000 * 1000 / dwSumInMicroSeconds ) : 0;
    DWORD const dwGensPer1000SecMeas = getMeasuredPerformance( );

    printLine( L"Model:  ", m_dwModelTime,              L"ms"    );
    printLine( L"Delay:  ", m_dwGenerationDelay * 1000, L"ms"    );
    printLine( L"Comp:   ", dwGensPer1000SecComp,       L"Gen/s" );
    printLine( L"Meas:   ", dwGensPer1000SecMeas,       L"Gen/s" );
    printLine( L"Display:", m_dwDisplayTime,            L"ms"    );
}
