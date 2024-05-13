// ScanViewer.ixx
//
// NNetViewer

module;

#include <optional>
#include <Windows.h>
#include "d2d1helper.h"

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
    		WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_BORDER|WS_VISIBLE,
			false,
			m_controller,
			nullptr
		);
		m_upGraphics->SetBackgroundColor(D2D1::ColorF::Ivory);
	}

	float AspectRatio() const { return m_pNMRI->GetRasterRect().AspectRatio(); }

private:
	void PaintGraphics() final
	{
		DrawScanArea(nullopt);
	}

	bool OnSize(PIXEL const width, PIXEL const height)
	{
		NNetWindow::OnSize(width, height);
		centerAndZoomRect();
		Notify(false);
		return true;
	}

	void centerAndZoomRect()
	{
		CenterAndZoomRect(GetCoord(), m_pNMRI->GetRasterRect(), 1.0f);
	}

	class controller: public NNetCommandHandler
	{
	public:
		void SetModelInterface(NNetModelReaderInterface const * const);

		bool HandleCommand(int const, LPARAM const, MicroMeterPnt const = NP_NULL) final
		{
			
			return false;
		};

	private:

	};				          

	controller m_controller;
};