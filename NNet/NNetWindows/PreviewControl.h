// PreviewControl.h
//
// NNetWindows

#pragma once

#include "Direct2D.h"
#include "MoreTypes.h"
#include "PixFpDimension.h"
#include "SignalGenerator.h"
#include "TimeGraph.h"

class PreviewControl : public TimeGraph
{
public:
	PreviewControl
	(
		HWND                   const hwndParent,
		SignalGenerator      * const pSigGen,
		PixFpDimension<fMicroSecs> * pHorzCoord,
		PixFpDimension<mV>         * pVertCoord
	)
	  : TimeGraph(hwndParent, pSigGen, pHorzCoord),
		m_pVertCoord(pVertCoord)
	{
		m_pVertCoord->RegisterObserver(*this);
	};

	~PreviewControl() final
	{
		m_pVertCoord->UnregisterObserver(*this);
	}

private:

	PixFpDimension<mV> * m_pVertCoord { nullptr };

	void DoPaint() final
	{
		m_upGraphics->FillRectangle(Convert2fPixelRect(GetClPixelRect()), D2D1::ColorF::Ivory);

		paintCurve
		(
			[this](fMicroSecs const t){ return pixPntVolt(t); }, 
			D2D1::ColorF::Black
		);
	}

	bool OnSize(PIXEL const width, PIXEL const height) final
	{
		TimeGraph::OnSize(width, height);
		Trigger();  // cause repaint
		return true;
	}

	fPixel getY (fPixel const fPix) const { return Convert2fPixel(GetClientWindowHeight()) - fPix; }
	fPixel yVolt(mV     const volt) const { return getY(m_pVertCoord->Transform2fPixelPos(volt)); }

	fPixelPoint pixPntVolt(fMicroSecs const t, mV     const v) const { return fPixelPoint(xTime(t), yVolt(v)); }
	fPixelPoint pixPntVolt(fMicroSecs const t) const { return pixPntVolt(t, m_pSigGen->GetVoltage(t)); }
};