// PreviewControl.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "PixFpDimension.h"
#include "TimeGraph.h"

class PreviewControl : public TimeGraph
{
public:
	PreviewControl
	(
		HWND                   const,
		SignalGenerator      * const,
		PixFpDimension<fMicroSecs> *,
		PixFpDimension<mV>         *
	);
	~PreviewControl() final;

private:

	PixFpDimension<mV> * m_pVertCoord { nullptr };

	void DoPaint() final;
	bool OnSize(PIXEL const, PIXEL const) final;

	fPixel getY (fPixel const fPix) const;
	fPixel yVolt(mV     const volt) const;

	fPixelPoint pixPntVolt(fMicroSecs const t, mV const v) const;
	fPixelPoint pixPntVolt(fMicroSecs const t) const;
};