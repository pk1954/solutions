// ScanViewer.cpp
//
// NNetViewer

module;

#include <optional>
#include <memory>
#include <Windows.h>

module ScanViewer;

import NNetPreferences;
import NNetWin32;

using std::nullopt;
using std::unique_ptr;

ScanViewer::ScanViewer
(
	HWND                             const hwndParent,
	NNetModelReaderInterface const * const pNMRI,
	mV                       const &       mVmax     
)
  :	m_mVmaxPixel(mVmax)
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
    m_upFiltered = m_pNMRI->GetScanImageC()->MeanFilter();
}

RawImage const& ScanViewer::GetImage() const
{
	return NNetPreferences::ApplyFilter()
		   ? *m_pNMRI->GetScanImageC() 
		   : *m_upFiltered.get();
}

float ScanViewer::AspectRatio() const 
{ 
	return m_pNMRI->GetRasterRect().AspectRatio(); 
}

void ScanViewer::PaintGraphics()
{
	m_pNMRI->DrawScanImage(m_context, GetImage(), m_mVmaxPixel, NNetPreferences::m_colorLutScan);
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
