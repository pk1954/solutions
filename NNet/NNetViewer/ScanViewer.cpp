// ScanViewer.cpp
//
// NNetViewer

module;

#include <Windows.h>

module ScanViewer;

import std;
import Tooltip;
import NNetPreferences;
import NNetWin32;

using std::nullopt;
using std::unique_ptr;
using std::make_unique;

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
    m_upFiltered    = m_pNMRI->GetScanImageC()->MeanFilter();
	wstring wstrTip = m_pNMRI->GetModelFilePath();
	m_upToolTip     = CreateWindowToolTip(wstrTip);
	m_upScanLut     = make_unique<D2D_ColorLUT>(&NNetPreferences::m_colorLutScan, GetGraphics());
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
	m_pNMRI->DrawScanImage(m_context, GetImage(), m_mVmaxPixel, *m_upScanLut.get());
}

bool ScanViewer::OnSize(PIXEL const width, PIXEL const height)
{
	NNetWindow::OnSize(width, height);
	centerAndZoomRect();
	m_upToolTip->Resize();
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
