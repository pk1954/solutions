// EventViewer.ixx
//
// NNetSignals

module;

#include <string>

export module NNetSignals:EventViewer;

import Types;
import PixFpDimension;
import :NNetTimeGraph;

using std::wstring;

export class EventViewer : public NNetTimeGraph
{
public:
	EventViewer(HWND const);

	wstring GetCaption() const final { return L"Event Viewer"; }

private:
	PixFpDimension<fMicroSecs> m_horzCoord;
	PixFpDimension<mV>         m_vertCoordVolt;

	void adjust(PixelRectSize const);
	void PaintGraphics() final;
	bool OnSize(PIXEL const, PIXEL const) final;
};