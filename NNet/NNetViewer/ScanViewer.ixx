// ScanViewer.ixx
//
// NNetViewer

module;

#include <optional>
#include <Windows.h>

export module ScanViewer;

import Types;
import NNetPreferences;
import NNetWin32;

using std::nullopt;

export class ScanViewer : public NNetWindow
{
public:
	void Start
	(
		HWND           const hwndParent, 
		bool           const bShowRefreshRateDialog,
		NNetCommandHandler & controller
	)
	{
		NNetWindow::Start
		(
			hwndParent, 
			WS_POPUPWINDOW | WS_CLIPSIBLINGS | WS_CAPTION | WS_SIZEBOX,
			bShowRefreshRateDialog,
			controller,
			nullptr
		);
	}

private:
	void PaintGraphics() final
	{
		DrawScanArea(nullopt);
	}

	void centerAndZoomRect()
	{
		CenterAndZoomRect(GetCoord(), m_pNMRI->GetScanAreaRect(), 1.0f);
	}
};