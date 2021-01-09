// win32_parameterDlg.cpp
//
// NNetWindows

#include "stdafx.h"

#include <sstream>
#include "Resource.h"
#include "win32_stdDialogBox.h"
#include "NNetParameters.h"
#include "NNetModelCommands.h"
#include "win32_parameterDlg.h"

using std::wstring;

ParameterDialog::ParameterDialog( ) 
  : BaseDialog( )
{ }

ParameterDialog::~ParameterDialog( )
{ }

void ParameterDialog::resetParameter   // refresh edit field with data from model
(
	HWND       const hwndEditField,
	ParameterType::Value const parameter
)
{
	StdDialogBox::SetParameterValue( hwndEditField, m_pParams->GetParameterValue( parameter ) );
}

void ParameterDialog::applyParameter  // read out edit field and write data to model
(
	HWND       const hwndEditField,
	ParameterType::Value const parameter
)
{
	float const fOldValue { m_pParams->GetParameterValue( parameter ) }; 
	float       fValue    { fOldValue }; 
	if ( StdDialogBox::Evaluate( hwndEditField, fValue ) && ! IsCloseToZero(fValue - fOldValue) )
		m_pCommands->SetParameter( parameter, fValue );
}

HWND ParameterDialog::createStaticField( HWND const hwndParent, wchar_t const * const text, int & iXpos, int const iYpos, int const iWidth )
{
	HWND hwnd = CreateWindow( L"Static", text, STYLE, iXpos, iYpos, iWidth, HEIGHT, hwndParent, 0, GetModuleHandle( nullptr ), 0 );
	iXpos += iWidth + HORZ_SPACE;
	return hwnd;
}

HWND ParameterDialog::createEditField( HWND const hwndParent, ParameterType::Value const parameter, int & iXpos, int const iYpos, int const iWidth )
{
	HWND hwnd =  CreateWindow( L"Edit", 0, STYLE|ES_RIGHT, iXpos, iYpos, iWidth, HEIGHT, hwndParent, 0, GetModuleHandle( nullptr ), 0 );
	resetParameter( hwnd, parameter );
	iXpos += iWidth + HORZ_SPACE;
	return hwnd;
}

HWND ParameterDialog::addParameter
(
	HWND       const hwndDlg,
	ParameterType::Value const parameter,
	int            & iYpos
)
{
	int  iXpos { 10 }; 

	HWND hwndName = createStaticField( hwndDlg, ParameterType::GetName( parameter ), iXpos, iYpos, 120 );
	HWND hwndEdit = createEditField  ( hwndDlg, parameter,                           iXpos, iYpos,  60 );
	HWND hwndUnit = createStaticField( hwndDlg, ParameterType::GetUnit( parameter ), iXpos, iYpos,  40 );

	iYpos += HEIGHT + VERT_SPACE;

	return hwndEdit;
}

void ParameterDialog::resetParameters( )  // refresh edit fields with data from model
{
	resetParameter( m_hwndPeakVoltage,      ParameterType::Value::peakVoltage    );
	resetParameter( m_hwndThreshold,        ParameterType::Value::threshold      );
	resetParameter( m_hwndPulseWidth,       ParameterType::Value::pulseWidth     );
	resetParameter( m_hwndRefractoryPeriod, ParameterType::Value::refractPeriod  );
	resetParameter( m_hwndTimeResolution,   ParameterType::Value::timeResolution );
	resetParameter( m_hwndPulseSpeed,       ParameterType::Value::pulseSpeed     );
}

void ParameterDialog::applyParameters( )  // read out edit field and write data to model
{
	applyParameter( m_hwndPeakVoltage,      ParameterType::Value::peakVoltage    );
	applyParameter( m_hwndThreshold,        ParameterType::Value::threshold      );
	applyParameter( m_hwndPulseWidth,       ParameterType::Value::pulseWidth     );
	applyParameter( m_hwndRefractoryPeriod, ParameterType::Value::refractPeriod  );
	applyParameter( m_hwndTimeResolution,   ParameterType::Value::timeResolution );
	applyParameter( m_hwndPulseSpeed,       ParameterType::Value::pulseSpeed     );
}

HWND ParameterDialog::createButton( HWND const hwndParent, wchar_t const * const text, int const x, int const y, int const w, int const h, HMENU const id )
{
	HWND hwnd = CreateWindow( L"Button", text, BS_DEFPUSHBUTTON | WS_TABSTOP | WS_CHILD | WS_VISIBLE, x, y, w, h, hwndParent, id, GetModuleHandle( nullptr ), 0 );
	return hwnd;
}

void ParameterDialog::Start
( 
	HWND                const hwndParent, 
	NNetModelCommands * const pCommands,	
	Param             * const pParams 
)
{
	HINSTANCE const hInstance { GetModuleHandle( nullptr ) };
	HWND      const hwndDlg   { StartBaseDialog( hwndParent, MAKEINTRESOURCE( IDM_PARAM_WINDOW ), nullptr ) };

	m_pCommands = pCommands;
	m_pParams   = pParams;

	int iYpos { 10 };
	m_hwndPeakVoltage      = addParameter( hwndDlg, ParameterType::Value::peakVoltage,    iYpos ); 
	m_hwndThreshold        = addParameter( hwndDlg, ParameterType::Value::threshold,      iYpos ); 
	m_hwndPulseWidth       = addParameter( hwndDlg, ParameterType::Value::pulseWidth,     iYpos ); 
	m_hwndRefractoryPeriod = addParameter( hwndDlg, ParameterType::Value::refractPeriod,  iYpos ); 
	m_hwndTimeResolution   = addParameter( hwndDlg, ParameterType::Value::timeResolution, iYpos ); 
	m_hwndPulseSpeed       = addParameter( hwndDlg, ParameterType::Value::pulseSpeed,     iYpos ); 

	createButton( hwndDlg, L"Apply", 140, iYpos, 50, 20, (HMENU)IDD_APPLY_PARAMETERS );
	createButton( hwndDlg, L"Reset", 200, iYpos, 50, 20, (HMENU)IDD_RESET_PARAMETERS );
}

void ParameterDialog::Stop( )
{
	m_pCommands = nullptr;
	m_pParams   = nullptr;
	DestroyWindow( );
}

bool ParameterDialog::OnCommand( WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint )
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

	default:
		break;
	}
	return BaseDialog::OnCommand( wParam, lParam );
}

bool ParameterDialog::UserProc( UINT const message, WPARAM const wParam, LPARAM const lParam )
{
	if ( message == WM_PAINT )
	{
		resetParameters( );
		return false;
	}

	return BaseDialog::CommonMessageHandler( message, wParam, lParam );
}
