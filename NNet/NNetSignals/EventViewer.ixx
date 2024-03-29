// EventViewer.ixx
//
// NNetSignals

module;

export module NNetSignals:EventViewer;

import :NNetTimeGraph;

export class EventViewer : public NNetTimeGraph
{
public:
private:
	void PaintGraphics() final;
};