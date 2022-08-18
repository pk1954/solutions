// SignalPreview.ixx
//
// NNetWindows

export module SignalPreview;

import BasicTypes;
import MoreTypes;
import PixelTypes;
import VoltageType;
import PixFpDimension;
import TimeGraph;
import BaseWindow;

export class SignalPreview : public TimeGraph
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

	fPixel getY (fPixel const) const;
	fPixel yVolt(mV     const) const;

	fPixelPoint pixPntVolt(fMicroSecs const, mV const) const;
};