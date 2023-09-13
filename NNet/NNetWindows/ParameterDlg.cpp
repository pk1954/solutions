// ParameterDlg.cpp
//
// NNetWindows

module;

#include <sstream>
#include <string>
#include "Resource.h"
#include <Windows.h>
#include "dwrite.h"

module NNetWin32:ParameterDialog;

import Win32_Util_Resource;
import Win32_Controls;
import Win32_Util;
import BaseDialog;
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
	::SetEditField(hwndEditField, m_pNMWI->GetParams().GetParameterValue(parameter));
}

void ParameterDialog::applyParameter  // read out edit field and write data to model
(
	HWND             const hwndEditField,
	ParamType::Value const parameter
)
{
	float const fOldValue { m_pNMWI->GetParams().GetParameterValue(parameter) }; 
	float       fValue    { fOldValue }; 
	bool  const bOK       { ::Evaluate(hwndEditField, fValue) };
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
	int  iXpos { LEFT_SPACE };

	HWND const hwndStatic = CreateStaticField(hwndDlg, ParamType::GetName(parameter), iXpos, iYpos, NAME_WIDTH, HEIGHT);
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
	resetParameter(m_hwndPulseFreqMax,   pulseFreqMax);
	resetParameter(m_hwndPeakVoltage,    neuronPeakVolt);
	resetParameter(m_hwndThreshold,      threshold);
	resetParameter(m_hwndSynapseDelay,   synapseDelay);
	resetParameter(m_hwndPulseSpeed,     pulseSpeed);
	resetParameter(m_hwndPulseWidth,     pulseWidth);
	resetParameter(m_hwndTimeResolution, timeResolution);
	resetParameter(m_hwndScanResolution, scanResolution);
}
										  
void ParameterDialog::applyParameters()  // read out edit field and write data to model
{										  
	using enum ParamType::Value;		  
	applyParameter(m_hwndPulseFreqMax,   pulseFreqMax);
	applyParameter(m_hwndPeakVoltage,    neuronPeakVolt);
	applyParameter(m_hwndThreshold,      threshold);
	applyParameter(m_hwndSynapseDelay,   synapseDelay);
	applyParameter(m_hwndPulseSpeed,     pulseSpeed);
	applyParameter(m_hwndPulseWidth,     pulseWidth);
	applyParameter(m_hwndTimeResolution, timeResolution);
	applyParameter(m_hwndScanResolution, scanResolution);
}

void ParameterDialog::Start(HWND const hwndParent)
{
	HWND const hwndDlg { StartBaseDialog(hwndParent, nullptr) };

	m_pTextFormatHeader = m_upGraphics->NewTextFormat(16.f);
	m_pTextFormatHeader->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);

	SetWindowText(L"Global parameters");
	SetWindowStyle(DS_3DLOOK|DS_CENTER|DS_MODALFRAME|DS_SHELLFONT|WS_CAPTION|WS_POPUP|WS_CLIPCHILDREN|WS_SYSMENU);

	int iYpos { VERT_SPACE + 16 };

	using enum ParamType::Value;

	m_hwndPeakVoltage = addParameter(hwndDlg, neuronPeakVolt, iYpos);
	m_hwndPulseWidth  = addParameter(hwndDlg, pulseWidth,     iYpos);
	m_hwndThreshold   = addParameter(hwndDlg, threshold,      iYpos);
	iYpos += VERT_BLOCK_SPACE;

	m_hwndSynapseDelay = addParameter(hwndDlg, synapseDelay, iYpos);
	iYpos += VERT_BLOCK_SPACE;

	m_hwndPulseSpeed     = addParameter(hwndDlg, pulseSpeed, iYpos);
	iYpos += VERT_BLOCK_SPACE;

	m_hwndPulseFreqMax   = addParameter(hwndDlg, pulseFreqMax,   iYpos);
	m_hwndTimeResolution = addParameter(hwndDlg, timeResolution, iYpos);
	iYpos += VERT_BLOCK_SPACE;

	m_hwndScanResolution = addParameter(hwndDlg, scanResolution, iYpos);
	iYpos += HEIGHT;

	CreateButton(hwndDlg, L"Apply", 120, iYpos, 50, 20, IDD_APPLY);
	CreateButton(hwndDlg, L"Reset", 180, iYpos, 50, 20, IDD_RESET);

	SetWindowSize(280_PIXEL, 430_PIXEL, true);
}

void ParameterDialog::PaintGraphics()
{
	m_fPixPosVert = 16._fPixel;
	paintHeader(3, L"Neuron");
	paintHeader(1, L"Synapse");
	paintHeader(1, L"Dendrite");
	paintHeader(2, L"General");
	paintHeader(1, L"Scan");
	refreshParameters();
}

void ParameterDialog::paintHeader
(
	int     const  iNrOfLines,
	wstring const& wstrText
)
{
	static fPixel const THICK   { 2._fPixel };
	static fPixel const CORNERS { 5._fPixel };
	static fPixel const VSIZE   { fPixel(HEIGHT) };
	static Color  const COL     { D2D1::ColorF::Black };

	fPixel const fPixBlockHeight { fPixel(Cast2Float(HEIGHT + VERT_SPACE) * iNrOfLines) };
	fPixel const fPixBottom      { m_fPixPosVert + fPixBlockHeight + 10._fPixel };
	m_upGraphics->DrawRoundedRectangle(fPixelRect(9._fPixel, m_fPixPosVert, 240._fPixel, fPixBottom), COL, CORNERS, THICK);
	
	fPixelRect rect { 16._fPixel, m_fPixPosVert - VSIZE + 3._fPixel, 85._fPixel, m_fPixPosVert + VSIZE };
	m_upGraphics->ClearRectangle(rect);
	
	rect.Move2Horz(2._fPixel);
	m_upGraphics->DisplayText(rect, wstrText.c_str(), m_pTextFormatHeader);

	m_fPixPosVert += fPixBlockHeight + fPixel(VERT_BLOCK_SPACE);
}

void ParameterDialog::Stop()
{
	BaseDialog::Stop();
	m_pNMWI = nullptr;
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
		SendCommand2Application(IDM_RESET_DYNAMIC_DATA);
		return true;

	case IDD_RESET:
		refreshParameters();
		return true;

	default:
		break;
	}
	return BaseDialog::OnCommand(wParam, lParam);
}

bool ParameterDialog::UserProc(UINT const message, WPARAM const wParam, LPARAM const lParam)
{
	HDC hdc = (HDC)wParam;
	//if (message == WM_CTLCOLORSTATIC)   // is called, but no effect
	//{
	//	SetBkColor(hdc, D2D1::ColorF::Red);
	//	HGDIOBJ brush { GetStockObject(DC_BRUSH) };
	//	return (INT_PTR)brush;
	//}
	if (message == WM_CTLCOLOREDIT)
	{
		SetBkColor(hdc, D2D1::ColorF::LightGreen);
		HGDIOBJ brush { GetStockObject(DC_BRUSH) };
		return (INT_PTR)brush;
	}

	return BaseDialog::CommonMessageHandler(message, wParam, lParam);
}