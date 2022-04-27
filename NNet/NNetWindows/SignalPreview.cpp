// SignalPreview.cpp 
//
// NNetModel

#include "stdafx.h"
#include "SignalGenerator.h"
#include "SignalPreview.h"

SignalPreview::SignalPreview
(
	BaseWindow           const & baseWinParent,
	PixFpDimension<fMicroSecs> & horzCoord,
	PixFpDimension<mV>         & vertCoord
)
  : TimeGraph(baseWinParent.GetWindowHandle(), &horzCoord),
	m_vertCoord(vertCoord)
{
	m_vertCoord.RegisterObserver(*this);
};

SignalPreview::~SignalPreview()
{
	m_vertCoord.UnregisterObserver(*this);
}

void SignalPreview::DoPaint()
{
	if (SignalGenerator * const pSigGen { GetSigGenActive() })
	if (Param     const * const pParams { GetParams() })
	{
		fMicroSecs const usSpikeWidth { pParams->SpikeWidth() };
		fMicroSecs const usResolution { pParams->TimeResolution() };

		m_upGraphics->FillRectangle(Convert2fPixelRect(GetClPixelRect()), D2D1::ColorF::Ivory);

		pSigGen->StartStimulus();
		PaintCurve
		(
			[this, usSpikeWidth, pSigGen](fMicroSecs const stimulusTime)
			{
				mV voltage = pSigGen->CalcVoltage(stimulusTime, usSpikeWidth);
				return pixPntVolt(stimulusTime, voltage); 
			}, 
			usResolution,
			D2D1::ColorF::Black
		);
	}
}

bool SignalPreview::OnSize(PIXEL const width, PIXEL const height)
{
	TimeGraph::OnSize(width, height);
	Trigger();  // cause repaint
	return true;
}

fPixel SignalPreview::getY(fPixel const fPix) const 
{ 
	return Convert2fPixel(GetClientWindowHeight()) - fPix; 
}

fPixel SignalPreview::yVolt(mV const volt) const 
{ 
	return getY(m_vertCoord.Transform2fPixelPos(volt)); 
}

fPixelPoint SignalPreview::pixPntVolt(fMicroSecs const t, mV const v) const 
{ 
	return fPixelPoint(xTime(t), yVolt(v)); 
}

fPixelPoint SignalPreview::pixPntVolt(fMicroSecs const t) const 
{ 
	return pixPntVolt(t, GetSigGenActive()->GetAmplitude(t));
}
