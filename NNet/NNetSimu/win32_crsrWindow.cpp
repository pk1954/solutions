// win32_crsrWindow.cpp :
//
// NNetWindows

#include "stdafx.h"
#include <iostream>
#include <sstream> 
#include <string> 
#include <iomanip>
#include "NNetModelInterface.h"
#include "Knot.h"
#include "Neuron.h"
#include "InputNeuron.h"
#include "Pipe.h"
#include "win32_util.h"
#include "win32_textBuffer.h"
#include "win32_NNetWindow.h"
#include "win32_crsrWindow.h"

using std::setprecision;

CrsrWindow::CrsrWindow( ) :
	TextWindow( )
{}

CrsrWindow::~CrsrWindow( )
{
	m_pNNetWindow = nullptr;
}

void CrsrWindow::Start
(
	HWND               const         hwndParent,
	NNetWindow         const * const pNNetWindow,
	NNetModelInterface const * const pModelInterface
) 
{
	m_pNNetWindow     = pNNetWindow;
	m_pModelInterface = pModelInterface;
	StartTextWindow
	(
		hwndParent, 
		PixelRect { 0_PIXEL, 300_PIXEL, 300_PIXEL, 500_PIXEL }, 
		L"CrsrWindow", 
		100, 
		true,
		nullptr
	);
//	::CreateWindowToolTip( GetWindowHandle(), L"Cursor window" );
}

void CrsrWindow::Stop( )
{
	TextWindow::StopTextWindow( );
	Show( false );
}

void CrsrWindow::printMicroMeter
( 
	TextBuffer     & textBuf,
	MicroMeter const um 
)
{
	wostringstream wBuffer;
	MicroMeter     umAbs { abs( um.GetValue() ) };
	wBuffer << std::fixed << setprecision(1);
	if ( umAbs < 1000.0_MicroMeter )
		wBuffer << um.GetValue() << L" um ";
	else if ( umAbs < 1000000.0_MicroMeter )
		wBuffer << um.GetValue() / 1000.0f << L" mm ";
	else
		wBuffer << um.GetValue() / 1000000.0f << L" m ";
	textBuf.printString( wBuffer.str() );
}

void CrsrWindow::printMilliSecs( TextBuffer & textBuf, MilliSecs const msec )
{
	wostringstream wBuffer;
	wBuffer << msec.GetValue() << L" msec";
	textBuf.printString( wBuffer.str() );
}

void CrsrWindow::DoPaint( TextBuffer & textBuf )
{
	HWND       const hwnd     { m_pNNetWindow->GetWindowHandle() };
	PixelPoint const pixPoint { Util::GetRelativeCrsrPosition( hwnd ) };

	if ( ! Util::IsInClientRect( hwnd, pixPoint ) )
	{
		textBuf.AlignLeft();
		textBuf.printString( L"Cursor not in model window" );
		return;
	}

	MicroMeterPoint const umPoint { m_pNNetWindow->PixelPoint2MicroMeterPoint( pixPoint ) };

	textBuf.printString( L"Position: " );
	printMicroMeter( textBuf, umPoint.GetX() );
	printMicroMeter( textBuf, umPoint.GetY() );
	textBuf.nextLine();

	ShapeId const id { m_pNNetWindow->GetHighlightedShapeId() };

	if ( IsUndefined( id ) )
		return;

	ShapeType const type { m_pModelInterface->GetShapeType( id ) };

	textBuf.printString( L"Shape #" );
	textBuf.printNumber( id.GetValue() );
	textBuf.nextLine( );
	textBuf.printString( L"Shape type:" ); 
	textBuf.printString( ShapeType::GetName( type.GetValue() ) ); 
	textBuf.nextLine( );

	mV potential { 0.0_mV };
	if ( type.IsPipeType( ) )
	{
		textBuf.printString( L"# segments:" );
		textBuf.printNumber( m_pModelInterface->GetNrOfSegments( id ) );
		potential = m_pModelInterface->GetVoltage( id, umPoint );
	}
	else 
	{
		potential = m_pModelInterface->GetVoltage( id );
	}

	textBuf.nextLine( L"potential " );
	textBuf.printFloat( potential.GetValue() );
	textBuf.printString( L" mV" );
	textBuf.nextLine( );

	if ( type.IsAnyNeuronType( ) )
	{
		if ( m_pModelInterface->HasTriggerSound( id ) )
		{
			textBuf.nextLine( L"trigger sound:" );
			printFrequency( textBuf, m_pModelInterface->GetTriggerSoundFrequency( id ) );
			printMilliSecs( textBuf, m_pModelInterface->GetTriggerSoundDuration ( id ) );
			textBuf.printString( L" msec" );
			textBuf.nextLine( );
		}
		if ( type.IsInputNeuronType( ) )
		{
			textBuf.printString( L"pulse freq: " );
			printFrequency( textBuf, m_pModelInterface->GetPulseFreq( id ) );
			textBuf.nextLine();
		}
	}
}
