// win32_performanceWindow.cpp :
//
// EvoWindows

#include "stdafx.h"
#include "Strsafe.h"
#include <chrono>
#include "util.h"
#include "InputNeuron.h"
#include "NNetModelInterface.h"
#include "win32_actionTimer.h"
#include "win32_NNetWorkThreadInterface.h"
#include "win32_performanceWindow.h"

using std::wostringstream;

//PerformanceWindow::PerformanceWindow( ) : 
//    TextWindow( )
//{ 
//}
//
//PerformanceWindow::~PerformanceWindow( )
//{
//}

void PerformanceWindow::Start
( 
	HWND                       const hwndParent,
	NNetModelInterface const * const pModelInterface,
	NNetWorkThreadInterface  * const pNNetWorkThreadInterface,
	ActionTimer              * const pDisplayTimer
)
{
	StartTextWindow
	( 
		hwndParent, 
		PixelRect { 0_PIXEL, 0_PIXEL, 300_PIXEL, 250_PIXEL }, 
		L"PerformanceWindow", 
		100,  // alpha
		TRUE,
		nullptr
	);
	m_pModelInterface          = pModelInterface;
	m_pNNetWorkThreadInterface = pNNetWorkThreadInterface;
	m_pAtDisplay               = pDisplayTimer;
	m_pNNetWorkThreadInterface->AddPerformanceObserver( this ); // notify me on computation performance changes 
}

void PerformanceWindow::Stop( )
{
	TextWindow::StopTextWindow( );
	Show( FALSE );
}

void PerformanceWindow::printMicroSecLine
(
	TextBuffer          & textBuf,
	wchar_t const * const pwch1, 
	fMicroSecs       const usDuration
)
{
	wostringstream wBuffer;
	float fPrintValue { usDuration.GetValue() };
	wBuffer << std::fixed << std::setprecision(1) << fPrintValue << L" �s";
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
	if ( m_pAtDisplay )
	{
		microseconds const usDisplayTime = m_pAtDisplay->GetSingleActionTime( );
		textBuf.printString( L"Display:" );
		textBuf.printString( L"" );
		textBuf.printAsMillisecs( usDisplayTime );
		textBuf.nextLine( );
	}

	if ( m_pNNetWorkThreadInterface )
	{
		fMicroSecs simuTime { m_pNNetWorkThreadInterface->GetSimulationTime( ) };
		fMicroSecs realTime { m_pNNetWorkThreadInterface->GetRealTimeTilStart( ) };
		fMicroSecs avail    { m_pNNetWorkThreadInterface->GetTimeAvailPerCycle( ) };
		fMicroSecs spent    { m_pNNetWorkThreadInterface->GetTimeSpentPerCycle( ) };
		printMicroSecLine( textBuf, L"simu time res:", m_pNNetWorkThreadInterface->GetSimuTimeResolution( ) );
		printFloatLine   ( textBuf, L"targ slowmo:", m_pNNetWorkThreadInterface->GetSlowMotionRatio( ), L"" );
		printMicroSecLine( textBuf, L"avail time:", avail );
		printMicroSecLine( textBuf, L"spent time:", spent );
		printFloatLine   ( textBuf, L"workload:",  CastToFloat( (spent / avail) * 100.0f ), L"%" );
		if ( simuTime > 0.0_MicroSecs )
			printFloatLine   ( textBuf, L"effect slomo:",  CastToFloat( realTime / simuTime ), L"" );
		// XXXXX
		//printIntLine( textBuf, L"# Shapes : ", m_pModelInterface->GetNrOf<Shape>() );
		//printIntLine( textBuf, L"# Input  : ", m_pModelInterface->GetNrOf<InputNeuron>() );
		//printIntLine( textBuf, L"# Neurons: ", m_pModelInterface->GetNrOf<Neuron>() );
		//printIntLine( textBuf, L"# Knots  : ", m_pModelInterface->GetNrOf<Knot>() );
		//printIntLine( textBuf, L"# Pipes  : ", m_pModelInterface->GetNrOf<Pipe>() );
	}
}
