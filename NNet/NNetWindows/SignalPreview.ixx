// SignalPreview.ixx
//
// NNetWindows

module;

#include <compare>

export module NNetWin32:SignalPreview;

import Types;
import PixFpDimension;
import :NNetTimeGraph;
import NNetModel;

export class SignalPreview : public NNetTimeGraph
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

	void PaintGraphics() final;
	bool OnSize(PIXEL const, PIXEL const) final;

	fPixel getY (fPixel const) const;
	fPixel yVolt(mV     const) const;

	fPixelPoint pixPntVolt(fMicroSecs const, mV const) const;
};