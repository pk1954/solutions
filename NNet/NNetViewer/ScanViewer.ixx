// ScanViewer.ixx
//
// NNetViewer

module;

#include <Windows.h>

export module ScanViewer;

import Types;
import NNetPreferences;
import NNetWin32;

export class ScanViewer : public NNetWindow
{
public:
	void Start
	(
		HWND           const hwndParent, 
		bool           const bShowRefreshRateDialog,
		NNetCommandHandler & controller,
		ScanMatrix   * const pScanMatrix
	)
	{
		NNetWindow::Start
		(
			hwndParent, 
			WS_POPUPWINDOW | WS_CLIPSIBLINGS | WS_CAPTION | WS_SIZEBOX,
			bShowRefreshRateDialog,
			controller,
			nullptr,
			pScanMatrix
		);
	}

private:
	void PaintGraphics() final
	{
		GetScanMatrix().DrawScanArea(m_context, NNetPreferences::m_colorLutScan, NNetPreferences::ApplyFilter());
	}

	void centerAndZoomRect()
	{
		CenterAndZoomRect(GetCoord(), m_pNMRI->GetScanAreaRect(), 1.0f);
	}
};