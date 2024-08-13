// ScanPanel.cpp
//
// NNetViewer

module;

#include <cassert>
#include <Windows.h>

module ScanPanel;

import std;
import Types;
import Raster;
import WinBasics;
import EventViewer;
import Resource;

using std::min;
using std::unique_ptr;
using std::make_unique;

static float const EVENT_VIEWER_HEIGHT { 0.1f };
static float const SCAN_WINDOW_HEIGHT  { 1.0f - EVENT_VIEWER_HEIGHT };

ScanPanel::ScanPanel
(
	HWND  const       hwndParent,
	unique_ptr<Model> upModel,
	PIXEL const       pixFrame,
	mV    const&      m_mVmaxPixel,     
	mV    const&      m_mVmaxAmplitude     
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
	m_upEventViewer = make_unique<EventViewer>(hwnd, &m_nmri, m_mVmaxAmplitude);
    m_upScanViewer  = make_unique<ScanViewer> (hwnd, &m_nmri, m_mVmaxPixel);
	m_upEventViewer->SetParentContextMenueMode(true);
	m_upScanViewer ->SetParentContextMenueMode(true);
}

void ScanPanel::Hide()
{
	ShowWindow(SW_HIDE);
}

PIXEL ScanPanel::PanelWidthFromHeight(PIXEL const pixPanelHeight) const
{
	PIXEL  const pixPanelHeightNet  { pixPanelHeight - m_pixFrame * 3 };
	fPixel const fPixPanelHeightNet { Convert2fPixel(pixPanelHeightNet) };
	fPixel const fPixScanHeight     { fPixPanelHeightNet * SCAN_WINDOW_HEIGHT };
	fPixel const fPixScanWidth      { fPixScanHeight / m_upScanViewer->AspectRatio() };
	PIXEL  const pixScanWidth       { Convert2PIXEL(fPixScanWidth) };
	PIXEL  const pixPanelWidth      { pixScanWidth + m_pixFrame * 2 };
	return pixPanelWidth;
}

PIXEL ScanPanel::PanelHeightFromWidth(PIXEL const pixPanelWidth) const
{
	PIXEL  const pixPanelWidthNet   { pixPanelWidth - m_pixFrame * 2 };
	fPixel const fPixScanWidth      { Convert2fPixel(pixPanelWidthNet) };
	fPixel const fPixScanHeight     { fPixScanWidth * m_upScanViewer->AspectRatio() };
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
	::ArrangeVertical(m_upEventViewer.get(), m_upScanViewer.get(), m_pixFrame);
	Notify(false);
	m_upEventViewer->Notify(false);
	m_upScanViewer->Notify(false);
	return true;
}

void ScanPanel::OnPaint()
{
	PAINTSTRUCT   ps;
	HDC           hDC { BeginPaint(&ps) };
	FillBackground(hDC, D2D1::ColorF::Green); 
	(void)EndPaint(&ps);
}

bool ScanPanel::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	switch (auto const wId = LOWORD(wParam))
	{
	case IDD_REMOVE_SCAN_PANEL:
		PostCommand2Parent(wParam, LPARAM(this));
		return true;

	default:
		break;
	}
	return BaseWindow::OnCommand(wParam, lParam, pixPoint);
}

LPARAM ScanPanel::AddContextMenuEntries(HMENU const hPopupMenu)
{
	AppendMenu(hPopupMenu, MF_STRING, IDD_REMOVE_SCAN_PANEL, L"Remove scan");
	return 0L;
}