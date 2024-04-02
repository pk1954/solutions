// EventViewer.ixx
//
// NNetSignals

module;

export module NNetSignals:EventViewer;

import Types;
import PixFpDimension;
import :NNetTimeGraph;

export class EventViewer : public NNetTimeGraph
{
public:
	EventViewer(HWND const);

private:
	PixFpDimension<fMicroSecs> m_horzCoord;

	void PaintGraphics() final;
	bool OnSize(PIXEL const, PIXEL const) final;
};