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
import EventViewer;

using std::min;
using std::unique_ptr;
using std::make_unique;

static float const EVENT_VIEWER_HEIGHT { 0.1f };
static float const SCAN_WINDOW_HEIGHT  { 1.0f - EVENT_VIEWER_HEIGHT };

ScanPanel::ScanPanel
(
	HWND const        hwndParent,
	unique_ptr<Model> upModel
)
{
	HWND hwnd = StartBaseWindow
	(
		hwndParent,
		CS_OWNDC|CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		L"ClassScanPanel",
		WS_POPUPWINDOW|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_CAPTION|WS_SIZEBOX|WS_VISIBLE,
		nullptr,
		nullptr
	);
	m_upModel = move(upModel);
	m_upEventViewer = make_unique<EventViewer>(hwnd);
    m_upEventViewer->SetModelInterface(m_pNMRI);
    m_upScanViewer = make_unique<ScanViewer>();
    m_upScanViewer->SetModelInterface(m_pNMRI);
	m_upScanViewer->Start(hwnd);
}

bool ScanPanel::OnSize(PIXEL const width, PIXEL const height)
{
	assert(m_upScanViewer);
	fPixel const fPixPanelHeight       { Convert2fPixel(height) };
	fPixel const fPixEventViewerHeight { fPixPanelHeight * EVENT_VIEWER_HEIGHT };
	PIXEL  const pixEventViewerHeight  { Convert2PIXEL(fPixEventViewerHeight) };

	ArrangeVertical(*m_upEventViewer.get(), *m_upScanViewer.get(), pixEventViewerHeight, true);

	BaseWindow::OnSize(width, height);
	return true;
}

float ScanPanel::AspectRatio() const
{
	return m_upScanViewer->AspectRatio() / SCAN_WINDOW_HEIGHT;
}
