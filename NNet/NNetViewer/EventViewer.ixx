// EventViewer.ixx
//
// NNetViewer

module;

#include <Windows.h>
#include <string>

export module EventViewer;

import Types;
import PixFpDimension;
import NNetModel;
import NNetSignals;

using std::wstring;

export class EventViewer : public NNetTimeGraph
{
public:
	EventViewer(HWND const);

	PIXEL GetFixedHeight() const final 
	{ 
		PIXEL const pixClientHeight { GetParentRootWindow()->GetClientWindowHeight() };
		return pixClientHeight / 10; 
	}

	wstring GetCaption() const final { return L"Event Viewer"; }

	//void OnMouseHover(WPARAM const wParam, LPARAM const lParam)
	//{
	//	int x = 42;
	//}

private:
	PixFpDimension<fMicroSecs> m_horzCoord;
	PixFpDimension<mV>         m_vertCoordVolt;

	fMicroSecs scanTime(EventType const) const;

	void adjust(PixelRectSize const);
	void adjustHorz(fPixel const);
	void adjustVert(fPixel const);
	void PaintGraphics() final;
	bool OnSize(PIXEL const, PIXEL const) final;
};