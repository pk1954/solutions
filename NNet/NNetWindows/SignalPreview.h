// SignalPreview.h
//
// NNetWindows

#pragma once

#include "TimeGraph.h"

import MoreTypes;
import PixFpDimension;

class BaseWindow;

class SignalPreview : public TimeGraph
{
public:
	SignalPreview
	(
		BaseWindow           const &,
		PixFpDimension<fMicroSecs> &,
		PixFpDimension<mV>         &
	);

	~SignalPreview() final;

private:

	PixFpDimension<mV> & m_vertCoord;

	void DoPaint() final;
	bool OnSize(PIXEL const, PIXEL const) final;

	fPixel getY (fPixel const fPix) const;
	fPixel yVolt(mV     const volt) const;

	fPixelPoint pixPntVolt(fMicroSecs const t, mV const v) const;
};