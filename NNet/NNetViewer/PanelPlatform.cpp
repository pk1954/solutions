// PanelPlatform.cpp
//
// NNetViewer

module;

#include <bit>
#include <cassert>
#include <vector>
#include <memory>
#include <algorithm>
#include <Windows.h>
#include <d2d1helper.h>
#include "Resource.h"

module PanelPlatform;

import SaveCast;
import Win32_Util;

using std::min;
using std::vector;
using std::unique_ptr;
using std::make_unique;
using std::bit_cast;
using std::distance;
using std::ranges::find_if;

static PIXEL const FRAME_WIDTH { 2_PIXEL };

PanelPlatform::PanelPlatform(HWND const hwndParent)
{
	HWND hwnd = StartBaseWindow
	(
		hwndParent,
		CS_OWNDC|CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		L"ClassPanelPlatform",
		WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE,
		nullptr,
		nullptr
	);
}

void PanelPlatform::calcTargetPanelList()
{
	PixelRectSize const& pixWinSize { GetClRectSize() };
	if ((pixWinSize.GetX() == 0_PIXEL) || (pixWinSize.GetY() == 0_PIXEL))
		return;

	long const nrOfPanels { Cast2Long(m_upPanels.size()) };
	if (nrOfPanels == 0)
		return;
	ScanPanel const * const pPanel { m_upPanels.front().get() };
	m_pixPanelWidth = 0_PIXEL;
	for (m_nrOfRows = 1; m_nrOfRows <= nrOfPanels; ++m_nrOfRows)
	{
		m_nrOfCols = nrOfPanels / m_nrOfRows;
		PIXEL  const pixPanelHeight   { pixWinSize.GetY() / m_nrOfRows };
		PIXEL  const pixPanelWidth1   { pixWinSize.GetX() / m_nrOfCols };
		PIXEL  const pixPanelWidth2   { pPanel->PanelWidthFromHeight(pixPanelHeight) };
		PIXEL  const pixPanelWidthMin { min(pixPanelWidth1, pixPanelWidth2) };
		if (pixPanelWidthMin > m_pixPanelWidth)
			m_pixPanelWidth = pixPanelWidthMin;
		else
			break;
	}
	m_pixPanelHeight = pPanel->PanelHeightFromWidth(m_pixPanelWidth);

	PIXEL pixPanelPosX(0_PIXEL);
	PIXEL pixPanelPosY(0_PIXEL);
	for (PixelRect &panelRect : m_panelRects)
	{
		panelRect = PixelRect
		(
			PixelPoint   (pixPanelPosX,    pixPanelPosY), 
			PixelRectSize(m_pixPanelWidth, m_pixPanelHeight)
		);
		pixPanelPosX += m_pixPanelWidth;
		if ((pixPanelPosX + m_pixPanelWidth) > pixWinSize.GetX())
		{
			pixPanelPosX = 0_PIXEL;
			pixPanelPosY += m_pixPanelHeight; 
		}
	}
}

void PanelPlatform::arrangePanels()
{
	calcTargetPanelList();
    for (int i = 0; i < m_upPanels.size(); ++i)
	{
		m_upPanels[i]->Move(m_panelRects[i], false);
	}
}

void PanelPlatform::recalc()
{
	m_mVmaxPixel     = 0.0_mV;
	m_mVmaxAmplitude = 0.0_mV;
	for (UpPanel &upPanel : m_upPanels)
	{
		mV const mVmaxPixel { upPanel->m_upScanViewer->GetImage().CalcMaxValue() };
		mV const mVmaxAmpl  { upPanel->m_upEventViewer->CalcMaxAmplitude() };
		m_mVmaxPixel     = max(mVmaxPixel, m_mVmaxPixel);
		m_mVmaxAmplitude = max(mVmaxAmpl,  m_mVmaxAmplitude);
	}
	for (UpPanel &upPanel : m_upPanels)
	{
		upPanel->m_upEventViewer->Notify(true);
		upPanel->m_upScanViewer ->Notify(true);
	}
}

void PanelPlatform::AddScan(unique_ptr<Model> upModel)
{
	UpPanel upPanel 
	{ 
		make_unique<ScanPanel>
		(
			GetWindowHandle(), 
			move(upModel), 
			FRAME_WIDTH,
			m_mVmaxPixel,
			m_mVmaxAmplitude
		) 
	};
	m_panelRects.push_back(PixelRect());
	m_upPanels.push_back(move(upPanel));
	arrangePanels();
	recalc();
	Notify(false);
}

void PanelPlatform::removeScan(ScanPanel *p)
{
	p->SendMessage(WM_CLOSE, 0, 0);
	auto iterUpPanels { find_if(m_upPanels, [p](UpPanel const &upPanel){ return upPanel.get() == p; }) };
	assert(iterUpPanels != m_upPanels.end());
	auto index { distance(m_upPanels.begin(), iterUpPanels) };
	m_upPanels  .erase(m_upPanels  .begin() + index);
	m_panelRects.erase(m_panelRects.begin() + index);
	arrangePanels();
	recalc();
	Notify(false);
}

bool PanelPlatform::OnSize(PIXEL const width, PIXEL const height)
{
	BaseWindow::OnSize(width, height);
	arrangePanels();
	Notify(true);
	return true;
}

void PanelPlatform::OnPaint()
{
	PAINTSTRUCT   ps;
	HDC           hDC { BeginPaint(&ps) };
	FillBackground(hDC, D2D1::ColorF::DarkGray); 
	(void)EndPaint(&ps);
}

bool PanelPlatform::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	switch (auto const wId = LOWORD(wParam))
	{
	case IDD_REMOVE_SCAN_PANEL:
		removeScan(bit_cast<ScanPanel*>(lParam));
		return true;

	default:
		break;
	}
	return BaseWindow::OnCommand(wParam, lParam, pixPoint);
}
