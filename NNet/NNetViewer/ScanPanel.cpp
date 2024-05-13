// ScanPanel.cpp
//
// NNetViewer

module;

#include <memory>
#include <cassert>
#include <algorithm>
#include <Windows.h>

module ScanPanel;

import Types;
import Raster;
import Win32_PIXEL;
import EventViewer;

using std::min;
using std::unique_ptr;
using std::make_unique;

static float const EVENT_VIEWER_HEIGHT { 0.1f };
static float const SCAN_WINDOW_HEIGHT  { 1.0f - EVENT_VIEWER_HEIGHT };

ScanPanel::ScanPanel
(
	HWND const        hwndParent,
	unique_ptr<Model> upModel,
	PIXEL const       pixFrame       
)
{
	HWND hwnd = StartBaseWindow
	(
		hwndParent,
		CS_OWNDC|CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		L"ClassScanPanel",
		WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE,
		nullptr,
		nullptr
	);
	m_pixFrame = pixFrame;
	m_upModel = move(upModel);
	m_nmri.SetModel(m_upModel.get());
	m_upEventViewer = make_unique<EventViewer>(hwnd);
    m_upEventViewer->SetModelInterface(&m_nmri);
    m_upScanViewer = make_unique<ScanViewer>();
    m_upScanViewer->SetModelInterface(&m_nmri);
	m_upScanViewer->Start(hwnd);
}

PIXEL ScanPanel::PanelWidthFromHeight(PIXEL const pixPanelHeight) const
{
	PIXEL  const pixPanelHeightNet  { pixPanelHeight - m_pixFrame * 3 };
	fPixel const fPixPanelHeightNet { Convert2fPixel(pixPanelHeightNet) };
	fPixel const fPixScanHeight     { fPixPanelHeightNet * SCAN_WINDOW_HEIGHT };
	fPixel const fPixScanWidth      { fPixScanHeight / AspectRatioScan() };
	PIXEL  const pixScanWidth       { Convert2PIXEL(fPixScanWidth) };
	PIXEL  const pixPanelWidth      { pixScanWidth + m_pixFrame * 2 };
	return pixPanelWidth;
}

PIXEL ScanPanel::PanelHeightFromWidth(PIXEL const pixPanelWidth) const
{
	PIXEL  const pixPanelWidthNet   { pixPanelWidth - m_pixFrame * 2 };
	fPixel const fPixScanWidth      { Convert2fPixel(pixPanelWidthNet) };
	fPixel const fPixScanHeight     { fPixScanWidth * AspectRatioScan() };
	fPixel const fPixPanelHeightNet { fPixScanHeight / SCAN_WINDOW_HEIGHT };
	PIXEL  const pixPanelHeightNet  { Convert2PIXEL(fPixPanelHeightNet) };
	PIXEL  const pixPanelHeight     { pixPanelHeightNet + m_pixFrame * 3 };
	return pixPanelHeight;
}

bool ScanPanel::OnSize(PIXEL const width, PIXEL const height)
{
	BaseWindow::OnSize(width, height);
	assert(m_upScanViewer);
	fPixel const fPixPanelHeight       { Convert2fPixel(height) };
	fPixel const fPixEventViewerHeight { fPixPanelHeight * EVENT_VIEWER_HEIGHT };
	PIXEL  const pixEventViewerHeight  { Convert2PIXEL(fPixEventViewerHeight) };
	::ArrangeVertical
	(
		m_upEventViewer->GetWindowHandle(),
		pixEventViewerHeight, 
		m_upScanViewer->GetWindowHandle(),
		m_pixFrame
	);
	Notify(false);
	return true;
}

void ScanPanel::OnPaint()
{
	PAINTSTRUCT   ps;
	HDC           hDC { BeginPaint(&ps) };
	FillBackground(hDC, D2D1::ColorF::Green); 
	(void)EndPaint(&ps);
}
