// ParameterDlg.cpp
//
// NNetWindows

module;

#include <sstream>
#include <string>
#include "Resource.h"
#include <Windows.h>
#include <windowsx.h>
#include "dwrite.h"

module NNetWin32:ParameterDialog;

import Win32_Util_Resource;
import Win32_Controls;
import Win32_Util;
import BaseDialog;
import NNetPreferences;
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

void ParameterDialog::resetParameter(ParamField& field) const
{
	::SetEditField(field.m_hwnd, m_pNMWI->GetParams().GetParameterValue(field.m_type));
}
void ParameterDialog::applyParameter(ParamField &field)   // read out edit field and write data to model
{
	float const fOldValue { m_pNMWI->GetParams().GetParameterValue(field.m_type) }; 
	float       fValue    { fOldValue }; 
	bool  const bOK       { ::Evaluate(field.m_hwnd, fValue) };
	if (bOK && ! IsCloseToZero(fValue - fOldValue))
		SetParameterCommand::Push(field.m_type, fValue);
}

void ParameterDialog::addParameter
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
	m_fields.push_back(ParamField(hwndEdit, parameter));
}

void ParameterDialog::refreshParameters()  // refresh edit fields with data from model
{
	for (auto& field : m_fields)
		resetParameter(field);
}
										  
void ParameterDialog::applyParameters()  // read out edit field and write data to model
{										  
	for (auto& field : m_fields)
		applyParameter(field);
}

void ParameterDialog::EnableAllEditFields()
{
	bool const bEnable { ! m_pNMWI->ScanImagePresent() };
	if (bEnable != m_bEnabled)
	{
		m_bEnabled = bEnable;
		for (auto& field : m_fields)
			Edit_Enable(field.m_hwnd, m_bEnabled);
	}
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

	addParameter(hwndDlg, neuronPeakVolt, iYpos);
	addParameter(hwndDlg, pulseWidth,     iYpos);
	addParameter(hwndDlg, threshold,      iYpos);
	iYpos += VERT_BLOCK_SPACE;

	addParameter(hwndDlg, synapseDelay, iYpos);
	iYpos += VERT_BLOCK_SPACE;

	addParameter(hwndDlg, pulseSpeed, iYpos);
	iYpos += VERT_BLOCK_SPACE;

	addParameter(hwndDlg, pulseFreqMax,   iYpos);
	addParameter(hwndDlg, timeResolution, iYpos);
	iYpos += VERT_BLOCK_SPACE;

	addParameter(hwndDlg, scanResolution, iYpos);
	addParameter(hwndDlg, pixelScanTime,  iYpos);
	iYpos += HEIGHT;

	CreateButton(hwndDlg, L"Apply", 120, iYpos, 50, 20, IDD_APPLY);
	CreateButton(hwndDlg, L"Reset", 180, iYpos, 50, 20, IDD_RESET);

	SetWindowSize(280_PIXEL, PIXEL(iYpos) + 70_PIXEL, true);
}

void ParameterDialog::PaintGraphics()
{
	m_fPixPosVert = 16._fPixel;
	paintHeader(3, L"Neuron");
	paintHeader(1, L"Synapse");
	paintHeader(1, L"Dendrite");
	paintHeader(2, L"General");
	paintHeader(2, L"Scan");
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
		SetBkColor(hdc, m_pNMWI->ScanImagePresent() ? D2D1::ColorF::LightGray : D2D1::ColorF::LightGreen);
		HGDIOBJ brush { GetStockObject(DC_BRUSH) };
		return (INT_PTR)brush;
	}

	return BaseDialog::CommonMessageHandler(message, wParam, lParam);
}