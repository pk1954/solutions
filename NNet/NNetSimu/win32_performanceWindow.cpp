// win32_performanceWindow.cpp :
//
// EvoWindows

#include "stdafx.h"
#include "Strsafe.h"
#include <chrono>
#include "util.h"
#include "InputNeuron.h"
#include "ComputeThread.h"
#include "SlowMotionRatio.h"
#include "NNetModelReaderInterface.h"
#include "win32_actionTimer.h"
#include "win32_performanceWindow.h"

using std::wostringstream;

void PerformanceWindow::Start
( 
	HWND                             const hwndParent,
	NNetModelReaderInterface const * const pModelInterface,
	ComputeThread                  * const pComputeThread,
	SlowMotionRatio          const * const pSlowMotionRatio,
	ActionTimer                    * const pDisplayTimer
)
{
	StartTextWindow
	( 
		hwndParent, 
		PixelRect { 0_PIXEL, 0_PIXEL, 300_PIXEL, 250_PIXEL }, 
		L"PerformanceWindow", 
		100,  // alpha
		true,
		nullptr
	);
	m_pSlowMotionRatio      = pSlowMotionRatio;
	m_pComputeThread        = pComputeThread;
	m_pDisplayTimer         = pDisplayTimer;
	m_pModelReaderInterface = pModelInterface;
}

void PerformanceWindow::Stop( )
{
	TextWindow::StopTextWindow( );
	Show( false );
}

void PerformanceWindow::printMicroSecLine
(
	TextBuffer          & textBuf,
	wchar_t const * const pwch1, 
	fMicroSecs      const usDuration
)
{
	wostringstream wBuffer;
	float fPrintValue { usDuration.GetValue() };
	wBuffer << std::fixed << std::setprecision(1) << fPrintValue << L" µs";
	textBuf.printString( pwch1 );
	textBuf.printString( L"" );
	textBuf.printString( wBuffer.str() );
	textBuf.nextLine   ( );
}

void PerformanceWindow::printIntLine
(
	TextBuffer          & textBuf,
	wchar_t const * const pwchBefore, 
	int             const iPrintValue
)
{
	wostringstream wBuffer;
	wBuffer << std::fixed << std::setprecision(1) << iPrintValue;
	textBuf.printString( pwchBefore );
	textBuf.printString( L"" );
	textBuf.printString( wBuffer.str() );
	textBuf.nextLine   ( );
}

void PerformanceWindow::printFloatLine
(
	TextBuffer          & textBuf,
	wchar_t const * const pwchBefore, 
	float           const fPrintValue,
	wchar_t const * const pwchAfter
)
{
	wostringstream wBuffer;
	wBuffer << std::fixed << std::setprecision(1) << fPrintValue << pwchAfter;
	textBuf.printString( pwchBefore );
	textBuf.printString( L"" );
	textBuf.printString( wBuffer.str() );
	textBuf.nextLine   ( );
}

void PerformanceWindow::DoPaint( TextBuffer & textBuf )
{      
	if ( m_pDisplayTimer )
	{
		microseconds const usDisplayTime = m_pDisplayTimer->GetSingleActionTime( );
		textBuf.printString( L"Display:" );
		textBuf.printString( L"" );
		textBuf.printAsMillisecs( usDisplayTime );
		textBuf.nextLine( );
	}

	if ( m_pComputeThread )
	{
		fMicroSecs simuTime { m_pComputeThread->GetSimulationTime( ) };
		fMicroSecs realTime { m_pComputeThread->GetRealTimeTilStart( ) };
		fMicroSecs avail    { m_pComputeThread->GetTimeAvailPerCycle( ) };
		fMicroSecs spent    { m_pComputeThread->GetTimeSpentPerCycle( ) };
		printMicroSecLine( textBuf, L"simu time res:", m_pComputeThread->GetSimuTimeResolution( ) );
		printFloatLine   ( textBuf, L"targ slowmo:", m_pSlowMotionRatio->GetRatio( ), L"" );
		printMicroSecLine( textBuf, L"avail time:", avail );
		printMicroSecLine( textBuf, L"spent time:", spent );
		printFloatLine   ( textBuf, L"workload:",  CastToFloat( (spent / avail) * 100.0f ), L"%" );
		if ( simuTime > 0.0_MicroSecs )
			printFloatLine   ( textBuf, L"effect slomo:", CastToFloat( realTime / simuTime ), L"" );
		printIntLine( textBuf, L"# Shapes : ", m_pModelReaderInterface->GetNrOfShapes() );
		printIntLine( textBuf, L"# Input  : ", m_pModelReaderInterface->GetNrOfInputNeurons() );
		printIntLine( textBuf, L"# Neurons: ", m_pModelReaderInterface->GetNrOfNeurons() );
		printIntLine( textBuf, L"# Knots  : ", m_pModelReaderInterface->GetNrOfKnots() );
		printIntLine( textBuf, L"# Pipes  : ", m_pModelReaderInterface->GetNrOfPipes() );
	}
}
