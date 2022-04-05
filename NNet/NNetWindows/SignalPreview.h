// SignalPreview.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "PixFpDimension.h"
#include "TimeGraph.h"

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

	static void SetParams(Param const * const pParams) { m_pParameters = pParams; }

private:

	inline static Param const * m_pParameters{ nullptr };

	PixFpDimension<mV> & m_vertCoord;

	void DoPaint() final;
	bool OnSize(PIXEL const, PIXEL const) final;

	fPixel getY (fPixel const fPix) const;
	fPixel yVolt(mV     const volt) const;

	fPixelPoint pixPntVolt(fMicroSecs const t, mV const v) const;
	fPixelPoint pixPntVolt(fMicroSecs const t) const;
};