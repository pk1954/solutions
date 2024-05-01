// ScanPanel.cpp
//
// NNetViewer

module;

#include <cassert>

module ScanPanel;

import Types;
import Raster;

static float const EVENT_VIEWER_HEIGHT { 0.1f };
static float const SCAN_WINDOW_HEIGHT  { 1.0f - EVENT_VIEWER_HEIGHT };

bool ScanPanel::OnSize(PIXEL const width, PIXEL const height)
{
	assert(m_upScanWindow);
	ScanMatrix  const & scanMatrix            { m_upScanWindow->GetScanMatrixC() };
	RasterPoint const   rasterSize            { scanMatrix.Size() };
	fPixel      const   fPixHeight            { Convert2fPixel(height) };
	fPixel      const   fPixScanWindowHeight  { fPixHeight * SCAN_WINDOW_HEIGHT };
	fPixel      const   fPixEventViewerHeight { fPixHeight * EVENT_VIEWER_HEIGHT };
	fPixel      const   fPixWidth             { Convert2fPixel(width) };
	float       const   fRatio                { scanMatrix.AspectRatio() };

	BaseWindow::OnSize(width, height);
	return true;
}
