// ScanViewer.cpp
//
// NNetViewer

module;

#include <optional>
#include <Windows.h>

module ScanViewer;

import NNetPreferences;
import NNetWin32;

using std::nullopt;

ScanViewer::ScanViewer
(
	HWND                             const hwndParent,
	NNetModelReaderInterface const * const pNMRI
)
{
	SetModelInterface(pNMRI);
	NNetWindow::Start
	(
		hwndParent, 
    	WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_BORDER|WS_VISIBLE,
		false,
		m_controller,
		nullptr
	);
	m_upGraphics->SetBackgroundColor(D2D1::ColorF::Ivory);
}

float ScanViewer::AspectRatio() const 
{ 
	return m_pNMRI->GetRasterRect().AspectRatio(); 
}

void ScanViewer::PaintGraphics()
{
	DrawScanAreaAll(nullopt);
}

bool ScanViewer::OnSize(PIXEL const width, PIXEL const height)
{
	NNetWindow::OnSize(width, height);
	centerAndZoomRect();
	Notify(false);
	return true;
}

void ScanViewer::centerAndZoomRect()
{
	CenterAndZoomRect(GetCoord(), m_pNMRI->GetRasterRect(), 1.0f);
}

bool ScanViewer::Controller::HandleCommand(int const, LPARAM const, MicroMeterPnt const)
{
			
	return false;
};
