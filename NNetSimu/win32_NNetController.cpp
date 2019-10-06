// win32_NNetController.cpp
//
// NNetSimu

#include "stdafx.h"
#include <chrono>
#include "Windowsx.h"
#include "Windows.h"
#include "Resource.h"
#include "BoolOp.h"
#include "LogarithmicTrackBar.h"
#include "SlowMotionRatio.h"
#include "win32_util.h"
#include "win32_simulationControl.h"
#include "win32_zoomControl.h"
#include "win32_aboutBox.h"
#include "win32_NNetAppWindow.h"
#include "win32_NNetWindow.h"
#include "win32_NNetEditor.h"
#include "win32_NNetWorkThreadInterface.h"
#include "win32_NNetController.h"

NNetController::NNetController
(
	NNetAppWindow           * const pAppWindow,
	NNetWindow              * const pNNetWindow,
	NNetEditorWindow        * const pNNetEditorWindow,
	WinManager              * const pWinManager,
	StatusBar               * const pStatusBar,
	NNetWorkThreadInterface * const pNNetWorkThreadInterface,
	SlowMotionRatio         * const pSlowMotionRatio
) 
  :	m_pAppWindow              ( nullptr ),
	m_pNNetWindow             ( pNNetWindow ),
	m_pNNetEditorWindow       ( pNNetEditorWindow ),
	m_pWinManager             ( pWinManager ),
	m_pStatusBar              ( pStatusBar ),
	m_pNNetWorkThreadInterface( pNNetWorkThreadInterface ),
	m_pSlowMotionRatio        ( pSlowMotionRatio ),
	m_hCrsrWait               ( LoadCursor( NULL, IDC_WAIT ) )
{
}

NNetController::~NNetController( )
{
	m_pNNetWorkThreadInterface = nullptr;
	m_pAppWindow               = nullptr;
	m_pWinManager              = nullptr;
	m_pSlowMotionRatio         = nullptr;
    m_pStatusBar               = nullptr;
}

bool NNetController::ProcessUIcommand( int const wmId, LPARAM const lParam )
{
	switch (wmId)
	{

	case IDM_TRACKBAR:
		switch ( lParam )
		{
		case IDM_ZOOM_TRACKBAR:
		{
			LONG const lLogicalPos = m_pStatusBar->GetTrackBarPos( IDM_ZOOM_TRACKBAR );
			LONG const lValue      = LogarithmicTrackbar::Value2TrackbarL( CastToLong( MAXIMUM_PIXEL_SIZE.GetValue()) ) - lLogicalPos;
			LONG const lPos        = LogarithmicTrackbar::TrackBar2ValueL( lValue );
			ProcessUIcommand( IDM_ZOOM_TRACKBAR, lPos );
		}
		break;

		default:
			assert( false );
		}
		break;

	case IDM_SLOWER:
		if ( ! m_pSlowMotionRatio->IncRatio( ) )
			MessageBeep( MB_ICONWARNING );
		break;

	case IDM_FASTER:
		if ( ! m_pSlowMotionRatio->DecRatio( ) )
			MessageBeep( MB_ICONWARNING );
		break;

	case IDM_ZOOM_TRACKBAR:  // comes from trackbar in statusBar
		(void)m_pNNetWindow->SetPixelSize( NanoMeter( static_cast<double>(lParam) ) );
		break;

	case IDM_FIT_ZOOM:
		//m_pNNetWindow->Fit2Rect( );
		//setSizeTrackBar( m_pNNetWindow->GetPixelSize() );
		break;

	case IDM_ZOOM_OUT:
	case IDM_ZOOM_IN:
		m_pNNetWindow->Zoom( wmId == IDM_ZOOM_IN );
		setSizeTrackBar( m_pNNetWindow->GetPixelSize() );
		break;

	case IDM_SET_ZOOM:
		m_pNNetWindow->SetPixelSize( NanoMeter( static_cast<double>(lParam) ) );
		setSizeTrackBar( NanoMeter(CastToLong(lParam)) );
		break;

	case IDM_REFRESH:
		m_pNNetWindow->Notify( lParam != 0 );
		break;

	case IDD_PULSE_RATE_DIALOG:
		m_pNNetWindow->PulseRateDialog();
		break;

	case IDD_PULSE_SPEED_DIALOG:
		m_pNNetWindow->PulseSpeedDialog();
		break;

	default:
		return FALSE; // command has not been processed
	}

	return TRUE;  // command has been processed
}

bool NNetController::ProcessModelCommand( int const wmId, LPARAM const lParam )
{
	switch ( wmId )
	{
	case IDM_HIGHLIGHT:
		m_pNNetWorkThreadInterface->PostHighlight( ShapeId( CastToUnsignedLong( lParam ) ) );
		break;

	case IDM_PULSE_FREQ:
	{
		ShapeId const shapeId   { Util::HiPart( lParam ) };
		Hertz   const pulseFreq { Util::LoPart( lParam ) };
		m_pNNetWorkThreadInterface->PostPulseFrequency( shapeId, pulseFreq );
	}
	break;

	case IDM_PULSE_SPEED:
	{
		ShapeId          const shapeId{ Util::HiPart( lParam ) };
		milliMeterPerSec const mmPs   { CastToLong( Util::LoPart( lParam ) ) };
		m_pNNetWorkThreadInterface->PostPulseSpeed( shapeId, mmPs );
	}
	break;

	default:
		return true;
	}

	return false;
}

void NNetController::setSizeTrackBar( NanoMeter const nmPixelSize )
{ 
	m_pStatusBar->SetTrackBarPos( IDM_ZOOM_TRACKBAR, CastToLong( LogarithmicTrackbar::Value2TrackbarD( nmPixelSize.GetValue() ) ) ); 
}
