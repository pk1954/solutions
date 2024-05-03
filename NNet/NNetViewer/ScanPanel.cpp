// ScanPanel.cpp
//
// NNetViewer

module;

#include <cassert>
#include <algorithm>

module ScanPanel;

import Types;
import Raster;

using std::min;

static float const EVENT_VIEWER_HEIGHT { 0.1f };
static float const SCAN_WINDOW_HEIGHT  { 1.0f - EVENT_VIEWER_HEIGHT };

bool ScanPanel::OnSize(PIXEL const width, PIXEL const height)
{
	assert(m_upScanWindow);
	fPixel const fPixPanelHeight       { Convert2fPixel(height) };
	fPixel const fPixPanelWidth        { Convert2fPixel(width) };
	float  const fRatio                { m_upScanWindow->GetScanMatrixC().AspectRatio() };
	fPixel const fPixMaxHeight1        { fPixPanelWidth / fRatio };
	fPixel const fPixMaxHeight2        { fPixPanelHeight * SCAN_WINDOW_HEIGHT };
	fPixel const fPixScanWindowHeight  { min(fPixMaxHeight1, fPixMaxHeight2) };
	fPixel const fPixEventViewerHeight { fPixPanelHeight - fPixScanWindowHeight };
	fPixel const fPixWidth             { fPixScanWindowHeight / fRatio };
	PIXEL  const pixEventViewerHeight  { Convert2PIXEL(fPixEventViewerHeight) };
	PIXEL  const pixScanWindowHeight   { Convert2PIXEL(fPixScanWindowHeight) };
	PIXEL  const pixWidth              { Convert2PIXEL(fPixWidth) };

	m_upEventViewer->Move(0_PIXEL, 0_PIXEL,                pixWidth, pixEventViewerHeight, true);
	m_upScanWindow ->Move(0_PIXEL, pixEventViewerHeight+1, pixWidth, pixScanWindowHeight,  true);

	BaseWindow::OnSize(width, height);
	return true;
}
