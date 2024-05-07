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
	void Start(HWND const hwndParent)
	{
		NNetWindow::Start
		(
			hwndParent, 
			WS_POPUPWINDOW | WS_CLIPSIBLINGS | WS_CAPTION | WS_SIZEBOX,
			false,
			m_controller,
			nullptr
		);
	}

	float AspectRatio() const { return m_pNMRI->GetScanAreaRect().AspectRatio(); }

private:
	void PaintGraphics() final
	{
		DrawScanArea(nullopt);
	}

	void centerAndZoomRect()
	{
		CenterAndZoomRect(GetCoord(), m_pNMRI->GetScanAreaRect(), 1.0f);
	}

	class controller: public NNetCommandHandler
	{
	public:
		void SetModelInterface(NNetModelReaderInterface * const);

		bool HandleCommand(int const, LPARAM const, MicroMeterPnt const = NP_NULL) final
		{
			return false;
		};

	private:

	};				          

	controller m_controller;

};