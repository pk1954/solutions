// win32_crsrWindow.cpp :
//
// NNetWindows

#include "stdafx.h"
#include <iostream>
#include <sstream> 
#include <string> 
#include <iomanip>
#include "win32_util.h"
#include "win32_textBuffer.h"
#include "win32_NNetWindow.h"
#include "win32_crsrWindow.h"

using std::wostringstream;
using std::setprecision;
using std::endl;

CrsrWindow::CrsrWindow( ) :
	TextWindow( ),
	m_pReadBuffer( nullptr ),
	m_pNNetWindow( nullptr )
{}

CrsrWindow::~CrsrWindow( )
{
	m_pReadBuffer = nullptr;
	m_pNNetWindow = nullptr;
}

void CrsrWindow::Start
(
	HWND               const hwndParent,
	NNetReadBuffer   * const pReadBuffer,
	NNetWindow const * const pNNetWindow
) 
{
	m_pReadBuffer = pReadBuffer;
	m_pNNetWindow = pNNetWindow;
	StartTextWindow
	(
		hwndParent, 
		PixelRect { 0_PIXEL, 300_PIXEL, 300_PIXEL, 500_PIXEL }, 
		L"CrsrWindow", 
		100, 
		TRUE,
		nullptr
	);
	m_pReadBuffer->RegisterObserver( this );
}

void CrsrWindow::Stop( )
{
	TextWindow::StopTextWindow( );
	Show( FALSE );
}

void CrsrWindow::printMicroMeter
( 
	TextBuffer     & textBuf,
	MicroMeter const um 
)
{
	wostringstream wBuffer;
	wBuffer << setprecision(3);
	if ( um < 1000.0_MicroMeter )
		wBuffer << um.GetValue() << L" um";
	else if ( um < 1000000.0_MicroMeter )
		wBuffer << um.GetValue() / 1000.0f << L" mm";
	else
		wBuffer << um.GetValue() / 1000000.0f << L" m";
	textBuf.printString( wBuffer.str() );
}

void CrsrWindow::DoPaint( TextBuffer & textBuf )
{
	textBuf.printString( L"Position:" );

	PixelPoint      const pixPoint{ Util::GetRelativeCrsrPosition( m_pNNetWindow->GetWindowHandle() ) };
	MicroMeterPoint const umPoint { m_pNNetWindow->PixelPoint2MicroMeterPoint( pixPoint ) };

	printMicroMeter( textBuf, umPoint.GetX() );
	printMicroMeter( textBuf, umPoint.GetY() );

	NNetModel const * pModel  = m_pReadBuffer->GetModel();
	ShapeId   const   shapeId = pModel->GetHighlightedShapeId();
	if ( IsDefined( shapeId ) )
	{
		Shape const * pShape = pModel->GetConstShape( shapeId );
		switch ( pShape->GetShapeType() )
		{
		case tShapeType::inputNeuron:
			break;

		case tShapeType::knot:
			break;

		case tShapeType::neuron:
			break;

		case tShapeType::outputNeuron:
			break;

		case tShapeType::pipeline:
		{
			Pipeline const * pPipeline = static_cast<Pipeline const *>( pShape );
			wostringstream wBuffer;
			textBuf.nextLine( L"Pipeline" );
			textBuf.nextLine( L"Segments: " );
			textBuf.printNumber( pPipeline->GetNrOfSegments( ) );
			textBuf.nextLine   ( pModel->GetParameterName( tParameter::pulseSpeed ) );
			textBuf.printFloat ( pModel->GetParameter    ( tParameter::pulseSpeed ) );
			textBuf.printString( pModel->GetParameterUnit( tParameter::pulseSpeed ) );
		}
		break;

		case tShapeType::undefined:
			assert( false );
			break;

		default:
			assert( false );
		}

	}
}
