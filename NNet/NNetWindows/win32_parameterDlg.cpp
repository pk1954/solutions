// win32_parameterDlg.cpp
//
// NNetWindows

#include "stdafx.h"

#include <sstream>
#include "Resource.h"
#include "win32_stdDialogBox.h"
#include "NNetModelWriterInterface.h"
#include "win32_parameterDlg.h"

using std::wstring;

ParameterDialog::ParameterDialog( ) 
  : BaseDialog( )
{ }

ParameterDialog::~ParameterDialog( )
{ }

void ParameterDialog::resetParameter
(
	HWND       const hwndEditField,
	tParameter const parameter
)
{
	StdDialogBox::SetParameterValue( hwndEditField, m_pParams->GetParameterValue( parameter ) );
}

void ParameterDialog::applyParameter
(
	HWND       const hwndEditField,
	tParameter const parameter
)
{
	float fValue { m_pParams->GetParameterValue( parameter ) }; 
	if ( StdDialogBox::Evaluate( hwndEditField, fValue ) )
		m_pModelWriterInterface->SetParameter( parameter, fValue );
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
	resetParameter( m_hwndPeakVoltage,      tParameter::peakVoltage    );
	resetParameter( m_hwndThreshold,        tParameter::threshold      );
	resetParameter( m_hwndPulseWidth,       tParameter::pulseWidth     );
	resetParameter( m_hwndRefractoryPeriod, tParameter::refractPeriod  );
	resetParameter( m_hwndTimeResolution,   tParameter::timeResolution );
	resetParameter( m_hwndPulseSpeed,       tParameter::pulseSpeed     );
}

void ParameterDialog::applyParameters( )
{
	applyParameter( m_hwndPeakVoltage,      tParameter::peakVoltage    );
	applyParameter( m_hwndThreshold,        tParameter::threshold      );
	applyParameter( m_hwndPulseWidth,       tParameter::pulseWidth     );
	applyParameter( m_hwndRefractoryPeriod, tParameter::refractPeriod  );
	applyParameter( m_hwndTimeResolution,   tParameter::timeResolution );
	applyParameter( m_hwndPulseSpeed,       tParameter::pulseSpeed     );
}

HWND ParameterDialog::createButton( HWND const hwndParent, wchar_t const * const text, int const x, int const y, int const w, int const h, HMENU const id )
{
	HWND hwnd = CreateWindow( L"Button", text, BS_DEFPUSHBUTTON | WS_TABSTOP | WS_CHILD | WS_VISIBLE, x, y, w, h, hwndParent, id, GetModuleHandle( nullptr ), 0 );
	return hwnd;
}

void ParameterDialog::Start
( 
	HWND                       const hwndParent, 
	NNetModelWriterInterface * const pModelWriterInterface,	
	Param                    * const pParams 
)
{
	HINSTANCE const hInstance { GetModuleHandle( nullptr ) };
	HWND      const hwndDlg   { StartBaseDialog( hwndParent, MAKEINTRESOURCE( IDM_PARAM_WINDOW ), nullptr ) };

	m_pModelWriterInterface = pModelWriterInterface;
	m_pParams = pParams;

	int iYpos { 10 };
	m_hwndPeakVoltage      = addParameter( hwndDlg, tParameter::peakVoltage,    iYpos ); 
	m_hwndThreshold        = addParameter( hwndDlg, tParameter::threshold,      iYpos ); 
	m_hwndPulseWidth       = addParameter( hwndDlg, tParameter::pulseWidth,     iYpos ); 
	m_hwndRefractoryPeriod = addParameter( hwndDlg, tParameter::refractPeriod,  iYpos ); 
	m_hwndTimeResolution   = addParameter( hwndDlg, tParameter::timeResolution, iYpos ); 
	m_hwndPulseSpeed       = addParameter( hwndDlg, tParameter::pulseSpeed,     iYpos ); 

	createButton( hwndDlg, L"Apply", 140, iYpos, 50, 20, (HMENU)IDD_APPLY_PARAMETERS );
	createButton( hwndDlg, L"Reset", 200, iYpos, 50, 20, (HMENU)IDD_RESET_PARAMETERS );
}

void ParameterDialog::Stop( )
{
	m_pModelWriterInterface = nullptr;
	m_pParams               = nullptr;
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
			return true;

		case IDD_RESET_PARAMETERS:
			resetParameters( );
			return true;

		case IDCANCEL:
			Show( false );
			return true;

		default:
			break;
		}
		PostCommand2Application( IDM_REFRESH, 0 );
	}
	break;

	default:
		break;
	}

	return BaseDialog::UserProc( message, wParam, lParam );
//	return false;
}
