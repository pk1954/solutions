// Win32_ParameterDlg.cpp
//
// NNetWindows

module;

#include <sstream>
#include "Resource.h"
#include <Windows.h>

module ParameterDialog;

import Win32_Util_Resource;
import StdDialogBox;
import Win32_Controls;
import Win32_Util;
import NNetModelCommands;
import NNetModel;

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
	Util::SetEditField(hwndEditField, m_pNMWI->GetParams().GetParameterValue(parameter));
}

void ParameterDialog::applyParameter  // read out edit field and write data to model
(
	HWND             const hwndEditField,
	ParamType::Value const parameter
)
{
	float const fOldValue { m_pNMWI->GetParams().GetParameterValue(parameter) }; 
	float       fValue    { fOldValue }; 
	bool  const bOK       { Util::Evaluate(hwndEditField, fValue) };
	if (bOK && ! IsCloseToZero(fValue - fOldValue))
		m_pCommands->SetParameter(parameter, fValue);
}

HWND ParameterDialog::addParameter
(
	HWND             const hwndDlg,
	ParamType::Value const parameter,
	int                  & iYpos
)
{
	static const int NAME_WIDTH { 120 };
	static const int EDIT_WIDTH {  60 };
	static const int UNIT_WIDTH {  40 };
	static int const HEIGHT     {  16 };

	int  iXpos { 10 }; 

	CreateStaticField(hwndDlg, ParamType::GetName(parameter), iXpos, iYpos, NAME_WIDTH, HEIGHT);
	iXpos += NAME_WIDTH + HORZ_SPACE;
	HWND const hwndEdit = CreateEditField(hwndDlg,            iXpos, iYpos, EDIT_WIDTH, HEIGHT);
	iXpos += EDIT_WIDTH + HORZ_SPACE;
	CreateStaticField(hwndDlg, ParamType::GetUnit(parameter), iXpos, iYpos, UNIT_WIDTH, HEIGHT);

	iYpos += HEIGHT + VERT_SPACE;

//	resetParameter(hwndEdit, parameter);

	return hwndEdit;
}

void ParameterDialog::resetParameters()  // refresh edit fields with data from model
{
	using enum ParamType::Value;
	resetParameter(m_hwndPeakVoltage,      neuronPeakVolt );
	resetParameter(m_hwndNeuronThreshold,  neuronThreshold);
	resetParameter(m_hwndSynapseThreshold, synapseThreshold);
	resetParameter(m_hwndSynapseBlockTime, synapseBlockTime);
	resetParameter(m_hwndSpikeWidth,       spikeWidth     );
	resetParameter(m_hwndRefractoryPeriod, refractPeriod  );
	resetParameter(m_hwndTimeResolution,   timeResolution );
	resetParameter(m_hwndPulseSpeed,       pulseSpeed     );
}

void ParameterDialog::applyParameters()  // read out edit field and write data to model
{
	using enum ParamType::Value;
	applyParameter(m_hwndPeakVoltage,      neuronPeakVolt );
	applyParameter(m_hwndNeuronThreshold,  neuronThreshold);
	applyParameter(m_hwndSynapseThreshold, synapseThreshold);
	applyParameter(m_hwndSynapseBlockTime, synapseBlockTime);
	applyParameter(m_hwndSpikeWidth,       spikeWidth     );
	applyParameter(m_hwndRefractoryPeriod, refractPeriod  );
	applyParameter(m_hwndTimeResolution,   timeResolution );
	applyParameter(m_hwndPulseSpeed,       pulseSpeed     );
}

void ParameterDialog::Start
(
	HWND                const hwndParent, 
	NNetModelCommands * const pCommands
)
{
	HWND const hwndDlg { StartBaseDialog(hwndParent, nullptr) };

	SetWindowSize(300_PIXEL, 300_PIXEL, false);
	SetWindowText(L"Global parameters");
	SetWindowStyle(DS_3DLOOK|DS_CENTER|DS_MODALFRAME|DS_SHELLFONT|WS_CAPTION|WS_POPUP|WS_SYSMENU);

	m_pCommands = pCommands;

	int iYpos { 10 };
	using enum ParamType::Value;
	m_hwndPeakVoltage      = addParameter(hwndDlg, neuronPeakVolt,   iYpos); 
	m_hwndNeuronThreshold  = addParameter(hwndDlg, neuronThreshold,  iYpos);
	m_hwndSynapseThreshold = addParameter(hwndDlg, synapseThreshold, iYpos);
	m_hwndSynapseBlockTime = addParameter(hwndDlg, synapseBlockTime, iYpos);
	m_hwndSpikeWidth       = addParameter(hwndDlg, spikeWidth,       iYpos);
	m_hwndRefractoryPeriod = addParameter(hwndDlg, refractPeriod,    iYpos); 
	m_hwndTimeResolution   = addParameter(hwndDlg, timeResolution,   iYpos); 
	m_hwndPulseSpeed       = addParameter(hwndDlg, pulseSpeed,       iYpos); 

	CreateButton(hwndDlg, L"Apply", 140, iYpos, 50, 20, IDD_APPLY);
	CreateButton(hwndDlg, L"Reset", 200, iYpos, 50, 20, IDD_RESET);
}

void ParameterDialog::Stop()
{
	m_pCommands = nullptr;
	m_pNMWI     = nullptr;
	DestroyWindow();
}

void ParameterDialog::SetModelInterface(NNetModelWriterInterface * const pNMWI)
{
	m_pNMWI = pNMWI;
}

bool ParameterDialog::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	switch (LOWORD(wParam))
	{
	case IDD_APPLY:
		applyParameters();
		SendCommand2Application(IDM_RESET_DYNAMIC_DATA, 0);
		return true;

	case IDD_RESET:
		resetParameters();
		SendCommand2Application(IDM_RESET_DYNAMIC_DATA, 0);
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
