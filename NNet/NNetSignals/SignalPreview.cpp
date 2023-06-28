// SignalPreview.cpp 
//
// NNetSignals

module NNetSignals:SignalPreview;

import Types;
import Direct2D;
import PixFpDimension;
import BaseWindow;
import GraphicsWindow;
import NNetModel;
import :NNetTimeGraph;

SignalPreview::SignalPreview
(
	BaseWindow           const & baseWinParent,
	PixFpDimension<fMicroSecs> & horzCoord,
	PixFpDimension<mV>         & vertCoord
)
  : NNetTimeGraph(baseWinParent.GetWindowHandle(), &horzCoord),
	m_vertCoord(vertCoord)
{
	m_vertCoord.RegisterObserver(*this);
};

SignalPreview::~SignalPreview()
{
	m_vertCoord.UnregisterObserver(*this);
}

void SignalPreview::PaintGraphics()
{
	if (SignalGenerator      * const pSigGen { GetSigGenSelected() })
	if (NNetParameters const * const pParams { GetParams() })
	{
		SigGenStaticData const & statData     { pSigGen->GetStaticData() };
		SigGenDynamicData        dynData      { };
		fMicroSecs       const   umPulseWidth { pParams->PulseWidth() };

		dynData.StartStimulus();
		PaintCurve
		(
			[this, &dynData, &statData, umPulseWidth](fMicroSecs const stimuTime)
			{
				mV const voltage { dynData.SetTime(statData, umPulseWidth, stimuTime) };
				return pixPntVolt(stimuTime, voltage); 
			}, 
			0.0_MicroSecs,
			getTime(xRight()),
			m_upGraphics->CreateBrush(D2D1::ColorF::Black),
			STD_WIDTH
		);
	}
}

bool SignalPreview::OnSize(PIXEL const width, PIXEL const height)
{
	NNetTimeGraph::OnSize(width, height);
	Trigger();  // cause repaint
	return true;
}

fPixel SignalPreview::getY(fPixel const fPix) const 
{ 
	return GetClientHeight() - fPix; 
}

fPixel SignalPreview::yVolt(mV const volt) const 
{ 
	return getY(m_vertCoord.Transform2fPixelPos(volt)); 
}

fPixelPoint SignalPreview::pixPntVolt(fMicroSecs const t, mV const v) const 
{ 
	return fPixelPoint(xTime(t), yVolt(v)); 
}
