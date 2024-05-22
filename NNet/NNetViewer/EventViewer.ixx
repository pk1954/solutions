// EventViewer.ixx
//
// NNetViewer

module;

#include <Windows.h>
#include <string>

export module EventViewer;

import Types;
import Tooltip;
import PixFpDimension;
import NNetModel;
import NNetSignals;

using std::wstring;

export class EventViewer : public NNetTimeGraph
{
public:
	EventViewer(HWND const, NNetModelReaderInterface const * const, mV const&);

	PIXEL   GetFixedHeight  () const final; 
	wstring GetCaption      () const final { return L"Event Viewer"; }
	mV      CalcMaxAmplitude() const;

private:

	PixFpDimension<fMicroSecs> m_horzCoord;
	PixFpDimension<mV>         m_vertCoordVolt;
	mV                  const& m_mVmaxAmplitude;
	UP_TTIP                    m_upToolTip;

    bool OnSize(PIXEL const, PIXEL const) final;

    fMicroSecs scanTime(EventType const) const;

	void adjust(PixelRectSize const);
	void adjustHorz(fPixel const);
	void adjustVert(fPixel const);
	void PaintGraphics() final;
};