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

import SaveCast;
import Win32_Util_Resource;
import WinManager;
import Win32_Controls;
import Win32_Util;
import WinManager;
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
	enableAllEditFields();
	BaseDialog::Notify(bImmediate);
}

void ParameterDialog::resetParameter(ParamField& field) const
{
	::SetWindowText(field.m_hwnd, Float2wstring(m_pNMWI->GetParams().GetParameterValue(field.m_type)));
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

void ParameterDialog::enableAllEditFields()
{
	bool const bEnable { ! m_pNMWI->ModelLocked() };
	if (bEnable != m_bEditParamsEnabled)
	{
		m_bEditParamsEnabled = bEnable;
		for (auto& field : m_fields)
			Edit_Enable(field.m_hwnd, m_bEditParamsEnabled);
	}
//	Edit_Enable(m_hwndFilter, m_bEditParamsEnabled);
}

void ParameterDialog::Start(HWND const hwndParent)
{
	HWND const hwndDlg { StartBaseDialog(hwndParent, nullptr) };

	m_hTextFormatHeader = m_upGraphics->NewTextFormat(16.f);
	m_hTextFormatHeader->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);

	SetWindowText(L"Global parameters");
	SetWindowStyle(DS_CENTER|DS_MODALFRAME|DS_SHELLFONT|WS_CAPTION|WS_POPUP|WS_CLIPCHILDREN|WS_SYSMENU);

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
	addParameter(hwndDlg, scanHorzPixels, iYpos);
	addParameter(hwndDlg, scanVertPixels, iYpos);
	addParameter(hwndDlg, pixelScanTime,  iYpos);
	addParameter(hwndDlg, nrOfScans,      iYpos);
	{
		int  iXpos { LEFT_SPACE };
		HWND const hwndStatic = CreateStaticField(hwndDlg, L"Total scan time", iXpos, iYpos, NAME_WIDTH, HEIGHT);
		iXpos += NAME_WIDTH + HORZ_SPACE;
		m_hwndScanTime = CreateStaticField(hwndDlg, L"", iXpos, iYpos, EDIT_WIDTH + HORZ_SPACE + UNIT_WIDTH, HEIGHT);
		iYpos += HEIGHT + VERT_SPACE;
	}

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
	paintHeader(7, L"Scan");
	refreshParameters();
	::SetWindowText(m_hwndScanTime, Format2wstring(m_pNMWI->TotalScanTime()));
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
	m_upGraphics->DisplayText(rect, wstrText.c_str(), m_hTextFormatHeader);

	m_fPixPosVert += fPixBlockHeight + fPixel(VERT_BLOCK_SPACE);
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
		WinManager::SendCommand2App(IDM_RESET_DYNAMIC_DATA);
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
		SetBkColor(hdc, m_pNMWI->ModelLocked() ? D2D1::ColorF::LightGray : D2D1::ColorF::LightGreen);
		HGDIOBJ brush { GetStockObject(DC_BRUSH) };
		return (INT_PTR)brush;
	}
	//if (message == WM_CTLCOLORBTN)       // is called, but no effect
	//{
	//	SetBkColor(hdc, m_pNMWI->ModelLocked() ? D2D1::ColorF::LightGray : D2D1::ColorF::LightGreen);
	//	HGDIOBJ brush { GetStockObject(DC_BRUSH) };
	//	return (INT_PTR)brush;
	//}

	return BaseDialog::CommonMessageHandler(message, wParam, lParam);
}