// ParameterDlg.cpp
//
// NNetWindows

module;

#include <sstream>
#include "Resource.h"
#include <Windows.h>

module NNetWin32:ParameterDialog;

import Win32_Util_Resource;
import StdDialogBox;
import Win32_Controls;
import Win32_Util;
import BaseWindow;
import NNetCommands;
import NNetModel;

using std::wstring;

ParameterDialog::ParameterDialog() 
  : BaseDialog()
{}

ParameterDialog::~ParameterDialog() = default;

void ParameterDialog::Notify(bool const bImmediate)
{
	refreshParameters();
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
		SetParameterCommand::Push(parameter, fValue);
}

HWND ParameterDialog::addParameter
(
	HWND             const hwndDlg,
	ParamType::Value const parameter,
	int                  & iYpos
)
{
	static const int NAME_WIDTH { 125 };
	static const int EDIT_WIDTH {  60 };
	static const int UNIT_WIDTH {  40 };
	static int const HEIGHT     {  14 };

	int  iXpos { 10 }; 

	CreateStaticField(hwndDlg, ParamType::GetName(parameter), iXpos, iYpos, NAME_WIDTH, HEIGHT);
	iXpos += NAME_WIDTH + HORZ_SPACE;
	HWND const hwndEdit = CreateEditField(hwndDlg,            iXpos, iYpos, EDIT_WIDTH, HEIGHT);
	iXpos += EDIT_WIDTH + HORZ_SPACE;
	CreateStaticField(hwndDlg, ParamType::GetUnit(parameter), iXpos, iYpos, UNIT_WIDTH, HEIGHT);

	iYpos += HEIGHT + VERT_SPACE;

	return hwndEdit;
}

void ParameterDialog::refreshParameters()  // refresh edit fields with data from model
{
	using enum ParamType::Value;
	resetParameter(m_hwndPulseFreqMax,    pulseFreqMax);
	resetParameter(m_hwndPeakVoltage,     neuronPeakVolt);
	resetParameter(m_hwndNeuronThreshold, neuronThreshold);
	resetParameter(m_hwndSynapseDelay,    synapseDelay);
	resetParameter(m_hwndPulseSpeed,      pulseSpeed);
	resetParameter(m_hwndPulseWidth,      pulseWidth);
	resetParameter(m_hwndTimeResolution,  timeResolution);
}										  
										  
void ParameterDialog::applyParameters()  // read out edit field and write data to model
{										  
	using enum ParamType::Value;		  
	applyParameter(m_hwndPulseFreqMax,    pulseFreqMax);
	applyParameter(m_hwndPeakVoltage,     neuronPeakVolt);
	applyParameter(m_hwndNeuronThreshold, neuronThreshold);
	applyParameter(m_hwndSynapseDelay,    synapseDelay);
	applyParameter(m_hwndPulseSpeed,      pulseSpeed);
	applyParameter(m_hwndPulseWidth,      pulseWidth);
	applyParameter(m_hwndTimeResolution,  timeResolution);
}

void ParameterDialog::Start
(
	HWND                const hwndParent, 
	NNetModelCommands * const pCommands
)
{
	HWND const hwndDlg { StartBaseDialog(hwndParent, nullptr) };

	StartGraphics();
	SetWindowSize(300_PIXEL, 300_PIXEL, false);
	SetWindowText(L"Global parameters");
	SetWindowStyle(DS_3DLOOK|DS_CENTER|DS_MODALFRAME|DS_SHELLFONT|WS_CAPTION|WS_POPUP|WS_SYSMENU);

	m_pCommands = pCommands;

	int iYpos { 10 };
	using enum ParamType::Value;
	m_hwndPulseFreqMax     = addParameter(hwndDlg, pulseFreqMax,    iYpos);
	m_hwndPeakVoltage      = addParameter(hwndDlg, neuronPeakVolt,  iYpos);
	m_hwndNeuronThreshold  = addParameter(hwndDlg, neuronThreshold, iYpos);
	m_hwndSynapseDelay     = addParameter(hwndDlg, synapseDelay,    iYpos);
	m_hwndPulseWidth       = addParameter(hwndDlg, pulseWidth,      iYpos);
	m_hwndTimeResolution   = addParameter(hwndDlg, timeResolution,  iYpos);
	m_hwndPulseSpeed       = addParameter(hwndDlg, pulseSpeed,      iYpos);

	CreateButton(hwndDlg, L"Apply", 140, iYpos, 50, 20, IDD_APPLY);
	CreateButton(hwndDlg, L"Reset", 200, iYpos, 50, 20, IDD_RESET);
}

void ParameterDialog::Stop()
{
	BaseDialog::Stop();
	m_pCommands = nullptr;
	m_pNMWI     = nullptr;
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
		refreshParameters();
		return true;

	default:
		break;
	}
	return BaseDialog::OnCommand(wParam, lParam);
}

void ParameterDialog::PaintGraphics()
{
	if (D2D_driver * pGraphics { GetGraphics() })
		pGraphics->FillBackground(D2D1::ColorF::Red);
	refreshParameters();
}
