// win32_parameterDlg.cpp
//
// NNetWindows

#include "stdafx.h"

#include <sstream> 
#include "Resource.h"
#include "NNetModel.h"
#include "win32_stdDialogBox.h"
#include "win32_NNetWorkThreadInterface.h"
#include "win32_parameterDlg.h"

using std::wstring;

ParameterDialog::ParameterDialog( NNetWorkThreadInterface * const pNNetWorkThreadInterface ) 
  : BaseDialog( ),
	m_pNNetModel( nullptr ),
	m_pNNetWorkThreadInterface( pNNetWorkThreadInterface ),
	m_hwndPeakVoltage     ( 0 ),
    m_hwndThreshold       ( 0 ),      
    m_hwndPulseWidth      ( 0 ),      
    m_hwndRefractoryPeriod( 0 ),
    m_hwndPulseSpeed      ( 0 ),
    m_hwndSignalLoss      ( 0 )
{ 
}

ParameterDialog::~ParameterDialog( )
{
	m_pNNetWorkThreadInterface = nullptr;
	m_pNNetModel = nullptr;
}

void ParameterDialog::resetParameter
(
	HWND       const hwndEditField,
	tParameter const parameter
)
{
	StdDialogBox::SetParameterValue( hwndEditField, m_pNNetModel->GetParameterValue( parameter ) );
}

void ParameterDialog::applyParameter
(
	HWND       const hwndEditField,
	tParameter const parameter
)
{
	float fValue { m_pNNetModel->GetParameterValue( parameter ) }; 
	if ( StdDialogBox::Evaluate( hwndEditField, fValue ) )
		m_pNNetWorkThreadInterface->PostSetParameter( parameter, fValue );
}

HWND ParameterDialog::createStaticField( HWND const hwndParent, wchar_t const * const text, int & iXpos, int const iYpos, int const iWidth )
{
	HWND hwnd = CreateWindow( L"Static", text, STYLE, iXpos, iYpos, iWidth, HEIGHT, hwndParent, 0, GetModuleHandle( nullptr ), 0 );
	iXpos += iWidth + HORZ_SPACE;
	return hwnd;
}

HWND ParameterDialog::createEditField( HWND const hwndParent, tParameter const parameter, int & iXpos, int const iYpos, int const iWidth )
{
	HWND hwnd =  CreateWindow( L"Edit", 0, STYLE|ES_RIGHT, iXpos, iYpos, iWidth, HEIGHT, hwndParent, 0, GetModuleHandle( nullptr ), 0 );
	resetParameter( hwnd, parameter );
	iXpos += iWidth + HORZ_SPACE;
	return hwnd;
}

HWND ParameterDialog::addParameter
(
	HWND       const hwndDlg,
	tParameter const parameter,
	int            & iYpos
)
{
	int  iXpos { 10 }; 

	HWND hwndName = createStaticField( hwndDlg, GetParameterName( parameter ), iXpos, iYpos, 120 );
	HWND hwndEdit = createEditField  ( hwndDlg, parameter,                     iXpos, iYpos,  60 );
	HWND hwndUnit = createStaticField( hwndDlg, GetParameterUnit( parameter ), iXpos, iYpos,  40 );

	iYpos += HEIGHT + VERT_SPACE;

	return hwndEdit;
}

void ParameterDialog::resetParameters( )
{
	resetParameter( m_hwndPeakVoltage,      tParameter::peakVoltage   );
	resetParameter( m_hwndThreshold,        tParameter::threshold     );
	resetParameter( m_hwndPulseWidth,       tParameter::pulseWidth    );
	resetParameter( m_hwndRefractoryPeriod, tParameter::refractPeriod );
	resetParameter( m_hwndPulseSpeed,       tParameter::pulseSpeed    );
	resetParameter( m_hwndSignalLoss,       tParameter::signalLoss    );
}

void ParameterDialog::applyParameters( )
{
	applyParameter( m_hwndPeakVoltage,      tParameter::peakVoltage   );
	applyParameter( m_hwndThreshold,        tParameter::threshold     );
	applyParameter( m_hwndPulseWidth,       tParameter::pulseWidth    );
	applyParameter( m_hwndRefractoryPeriod, tParameter::refractPeriod );
	applyParameter( m_hwndPulseSpeed,       tParameter::pulseSpeed    );
	applyParameter( m_hwndSignalLoss,       tParameter::signalLoss    );
}

HWND ParameterDialog::createButton( HWND const hwndParent, wchar_t const * const text, int const x, int const y, int const w, int const h, HMENU const id )
{
	HWND hwnd = CreateWindow( L"Button", text, BS_DEFPUSHBUTTON | WS_TABSTOP | WS_CHILD | WS_VISIBLE, x, y, w, h, hwndParent, id, GetModuleHandle( nullptr ), 0 );
	return hwnd;
}

void ParameterDialog::Start( HWND const hwndParent,	NNetModel * const pModel )
{
	HINSTANCE const hInstance { GetModuleHandle( nullptr ) };
	HWND      const hwndDlg   { StartBaseDialog( hwndParent, MAKEINTRESOURCE( IDM_PARAM_WINDOW ), nullptr ) };

	m_pNNetModel = pModel;

	int iYpos { 10 };
	m_hwndPeakVoltage      = addParameter( hwndDlg, tParameter::peakVoltage,   iYpos ); 
	m_hwndThreshold        = addParameter( hwndDlg, tParameter::threshold,     iYpos ); 
	m_hwndPulseWidth       = addParameter( hwndDlg, tParameter::pulseWidth,    iYpos ); 
	m_hwndRefractoryPeriod = addParameter( hwndDlg, tParameter::refractPeriod, iYpos ); 
	m_hwndPulseSpeed       = addParameter( hwndDlg, tParameter::pulseSpeed,    iYpos ); 
	m_hwndSignalLoss       = addParameter( hwndDlg, tParameter::signalLoss,    iYpos ); 

	createButton( hwndDlg, L"Apply", 140, iYpos, 50, 20, (HMENU)IDD_APPLY_PARAMETERS );
	createButton( hwndDlg, L"Reset", 200, iYpos, 50, 20, (HMENU)IDD_RESET_PARAMETERS );

	m_pNNetModel->AddParameterObserver( this );
}

void ParameterDialog::Stop( )
{
	DestroyWindow( );
}

LRESULT ParameterDialog::UserProc( UINT const message, WPARAM const wParam, LPARAM const lParam )
{
	switch (message)
	{
	case WM_PAINT:
		resetParameters( );
		break;

	case WM_COMMAND:
	{
		WORD const wId = LOWORD(wParam);
		switch ( wId )
		{

		case IDD_APPLY_PARAMETERS:
			applyParameters( );
			return TRUE;

		case IDD_RESET_PARAMETERS:
			resetParameters( );
			return TRUE;

		case IDCANCEL:
			Show( FALSE );
			return TRUE;

		default:
			break;
		}
		PostCommand2Application( IDM_REFRESH, 0 );
	}
	break;

	default:
		break;
	}

	return FALSE;
}
