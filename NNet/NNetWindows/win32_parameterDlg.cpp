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

ParameterDialog::ParameterDialog() 
  : BaseDialog()
{ }

ParameterDialog::~ParameterDialog() = default;

void ParameterDialog::Notify(bool const bImmediate)
{
	resetParameters();
	BaseDialog::Notify(bImmediate);
}

void ParameterDialog::resetParameter   // refresh edit field with data from model
(
	HWND             const hwndEditField,
	ParamType::Value const parameter
) const
{
	StdDialogBox::SetParameterValue(hwndEditField, m_pParams->GetParameterValue(parameter));
}

void ParameterDialog::applyParameter  // read out edit field and write data to model
(
	HWND             const hwndEditField,
	ParamType::Value const parameter
)
{
	float const fOldValue { m_pParams->GetParameterValue(parameter) }; 
	float       fValue    { fOldValue }; 
	if (StdDialogBox::Evaluate(hwndEditField, fValue) && ! IsCloseToZero(fValue - fOldValue))
		m_pCommands->SetParameter(parameter, fValue);
}

HWND ParameterDialog::createStaticField(HWND const hwndParent, wchar_t const * const text, int & iXpos, int const iYpos, int const iWidth)
{
	HWND hwnd = CreateWindow(L"Static", text, STYLE, iXpos, iYpos, iWidth, HEIGHT, hwndParent, 0, GetModuleHandle(nullptr), 0);
	iXpos += iWidth + HORZ_SPACE;
	return hwnd;
}

HWND ParameterDialog::createEditField(HWND const hwndParent, ParamType::Value const parameter, int & iXpos, int const iYpos, int const iWidth)
{
	HWND hwnd = CreateWindow(L"Edit", 0, STYLE|ES_RIGHT, iXpos, iYpos, iWidth, HEIGHT, hwndParent, 0, GetModuleHandle(nullptr), 0);
	resetParameter(hwnd, parameter);
	iXpos += iWidth + HORZ_SPACE;
	return hwnd;
}

HWND ParameterDialog::addParameter
(
	HWND             const hwndDlg,
	ParamType::Value const parameter,
	int                  & iYpos
)
{
	int  iXpos { 10 }; 

	HWND const hwndName = createStaticField(hwndDlg, ParamType::GetName(parameter), iXpos, iYpos, 120);
	HWND const hwndEdit = createEditField  (hwndDlg, parameter,                     iXpos, iYpos,  60);
	HWND const hwndUnit = createStaticField(hwndDlg, ParamType::GetUnit(parameter), iXpos, iYpos,  40);

	iYpos += HEIGHT + VERT_SPACE;

	return hwndEdit;
}

void ParameterDialog::resetParameters()  // refresh edit fields with data from model
{
	using enum ParamType::Value;
	resetParameter(m_hwndBaseFrequency,    baseFrequency );
	resetParameter(m_hwndPeakVoltage,      peakVoltage   );
	resetParameter(m_hwndThreshold,        threshold     );
	resetParameter(m_hwndPulseWidth,       pulseWidth    );
	resetParameter(m_hwndRefractoryPeriod, refractPeriod );
	resetParameter(m_hwndTimeResolution,   timeResolution);
	resetParameter(m_hwndPulseSpeed,       pulseSpeed    );
}

void ParameterDialog::applyParameters()  // read out edit field and write data to model
{
	using enum ParamType::Value;
	applyParameter(m_hwndBaseFrequency,    baseFrequency );
	applyParameter(m_hwndPeakVoltage,      peakVoltage   );
	applyParameter(m_hwndThreshold,        threshold     );
	applyParameter(m_hwndPulseWidth,       pulseWidth    );
	applyParameter(m_hwndRefractoryPeriod, refractPeriod );
	applyParameter(m_hwndTimeResolution,   timeResolution);
	applyParameter(m_hwndPulseSpeed,       pulseSpeed    );
}

HWND ParameterDialog::createButton(HWND const hwndParent, wchar_t const * const text, int const x, int const y, int const w, int const h, int const id)
{
	HWND hwnd = CreateWindow(L"Button", text, BS_DEFPUSHBUTTON|WS_TABSTOP|WS_CHILD|WS_VISIBLE, x, y, w, h, hwndParent, (HMENU)id, GetModuleHandle(nullptr), 0);
	return hwnd;
}

void ParameterDialog::Start
(
	HWND                const hwndParent, 
	NNetModelCommands * const pCommands,	
	Param             * const pParams 
)
{
	HWND const hwndDlg { StartBaseDialog(hwndParent, MAKEINTRESOURCE(IDM_PARAM_WINDOW), nullptr) };

	m_pCommands = pCommands;
	m_pParams   = pParams;

	int iYpos { 10 };
	using enum ParamType::Value;
	m_hwndBaseFrequency    = addParameter(hwndDlg, baseFrequency,  iYpos); 
	m_hwndPeakVoltage      = addParameter(hwndDlg, peakVoltage,    iYpos); 
	m_hwndThreshold        = addParameter(hwndDlg, threshold,      iYpos); 
	m_hwndPulseWidth       = addParameter(hwndDlg, pulseWidth,     iYpos); 
	m_hwndRefractoryPeriod = addParameter(hwndDlg, refractPeriod,  iYpos); 
	m_hwndTimeResolution   = addParameter(hwndDlg, timeResolution, iYpos); 
	m_hwndPulseSpeed       = addParameter(hwndDlg, pulseSpeed,     iYpos); 

	createButton(hwndDlg, L"Apply", 140, iYpos, 50, 20, IDD_APPLY_PARAMETERS);
	createButton(hwndDlg, L"Reset", 200, iYpos, 50, 20, IDD_RESET_PARAMETERS);
}

void ParameterDialog::Stop()
{
	m_pCommands = nullptr;
	m_pParams   = nullptr;
	DestroyWindow();
}

bool ParameterDialog::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	auto const wId { LOWORD(wParam) };
	switch (wId)
	{
	case IDD_APPLY_PARAMETERS:
		applyParameters();
		return true;

	case IDD_RESET_PARAMETERS:
		resetParameters();
		return true;

	default:
		break;
	}
	return BaseDialog::OnCommand(wParam, lParam);
}

bool ParameterDialog::UserProc(UINT const message, WPARAM const wParam, LPARAM const lParam)
{
	if (message == WM_PAINT)
	{
		resetParameters();
		return false;
	}

	return BaseDialog::CommonMessageHandler(message, wParam, lParam);
}
