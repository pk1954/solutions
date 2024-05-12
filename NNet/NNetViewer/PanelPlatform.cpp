// PanelPlatform.cpp
//
// NNetViewer

module;

#include <vector>
#include <algorithm>
#include <Windows.h>
#include <d2d1helper.h>

module PanelPlatform;

import SaveCast;
import Win32_Util;

using std::min;
using std::vector;

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

void PanelPlatform::arrangePanels(PixelRectSize const& pixWinSize)
{
	long const nrOfPanels { Cast2Long(m_panelList.size()) };
	if (nrOfPanels == 0)
		return;
	float const fRatioPanel   { m_panelList.front()->AspectRatio() };
	m_pixPanelWidth = 0_PIXEL;
	for (m_nrOfRows = 1; m_nrOfRows <= nrOfPanels; ++m_nrOfRows)
	{
		m_nrOfCols = nrOfPanels / m_nrOfRows;
		PIXEL  const pixPanelWidth1   { pixWinSize.GetX() / m_nrOfCols };
		PIXEL  const pixPanelHeight   { pixWinSize.GetY() / m_nrOfRows };
		fPixel const fPixPanelWidth2  { Convert2fPixel(pixPanelHeight) / fRatioPanel };
		PIXEL  const pixPanelWidth2   { Convert2PIXEL(fPixPanelWidth2) };
		PIXEL  const pixPanelWidthMin { min(pixPanelWidth1, pixPanelWidth2) };
		if (pixPanelWidthMin > m_pixPanelWidth)
			m_pixPanelWidth = pixPanelWidthMin;
		else
			break;
	}
	m_pixPanelHeight = Convert2PIXEL(fPixel(Cast2Float(m_pixPanelWidth.GetValue()) * fRatioPanel));

	PixelRectSize const pixPanelSize(m_pixPanelWidth, m_pixPanelHeight);
	PIXEL               pixPanelPosX(0_PIXEL);
	PIXEL               pixPanelPosY(0_PIXEL);
	for (UpPanel const &upPanel : m_panelList)
	{
		PixelPoint const pixPanelPos(pixPanelPosX, pixPanelPosY);
		PixelRect  const rect(pixPanelPos, pixPanelSize);
		upPanel->Move(rect, false);
		if ((pixPanelPosX += m_pixPanelWidth) > pixWinSize.GetX())
		{
			pixPanelPosX = 0_PIXEL;
			pixPanelPosY += m_pixPanelHeight; 
		}
	}
}

void PanelPlatform::AddScan(UpPanel panel)
{
	m_panelList.push_back(move(panel));
	arrangePanels(GetClRectSize());
}

vector<UpPanel>::iterator PanelPlatform::findPanel(PixelPoint const& pixPos)
{
	long const nrOfPanels { Cast2Long(m_panelList.size()) };
	if (nrOfPanels == 0)
		return m_panelList.end();
	long const col { pixPos.GetX() / m_pixPanelWidth };
	long const row { pixPos.GetY() / m_pixPanelWidth };
	if ((col >= m_nrOfCols)||(row >= m_nrOfRows))
		return m_panelList.end();
	long const index { row * m_nrOfCols + col };
	return m_panelList.begin() + index;
}

bool PanelPlatform::OnSize(PIXEL const width, PIXEL const height)
{
	arrangePanels(PixelRectSize(width, height));
	return true;
}

void PanelPlatform::OnPaint()
{
	PAINTSTRUCT   ps;
	HDC           hDC { BeginPaint(&ps) };
	FillBackground(hDC, D2D1::ColorF::DarkGray); 
	(void)EndPaint(&ps);
}
