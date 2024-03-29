// SignalPreview.ixx
//
// NNetSignals

export module NNetSignals:SignalPreview;

import Types;
import PixFpDimension;
import NNetModel;
import :NNetTimeGraph;

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