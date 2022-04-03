// PreviewControl.cpp 
//
// NNetModel

#include "stdafx.h"
#include "SignalGenerator.h"
#include "PreviewControl.h"

PreviewControl::PreviewControl
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

PreviewControl::~PreviewControl()
{
	m_vertCoord.UnregisterObserver(*this);
}

void PreviewControl::DoPaint()
{
	fMicroSecs const usSpikeWidth { m_pSigGen->GetParamsC().SpikeWidth() };
	float      const factorW      { 1.0f / usSpikeWidth.GetValue() };
	fMicroSecs       usLastPulse  { 0.0_MicroSecs };
	fMicroSecs const usResolution { m_pSigGen->GetParamsC().TimeResolution() };

	m_upGraphics->FillRectangle(Convert2fPixelRect(GetClPixelRect()), D2D1::ColorF::Ivory);

	PaintCurve
	(
		[this, &usLastPulse, usSpikeWidth, factorW](fMicroSecs const t)
		{ 
			fMicroSecs const usPulse { PulseDuration(m_pSigGen->GetFrequency(t)) };
			fMicroSecs const time    { t - usLastPulse };
			mV               voltage { 0.0_mV };
			if (time > usPulse)
				usLastPulse = t;
			if (time <= usSpikeWidth)
			{
				mV const amplitude { m_pSigGen->GetAmplitude(t) };
				mV const mVfactor  { amplitude * 4.0f * factorW };
				voltage = mVfactor * time.GetValue() * (1.0f - time.GetValue() * factorW);
			}
			return pixPntVolt(t, voltage); 
		}, 
		usResolution,
		D2D1::ColorF::Black
	);
}

bool PreviewControl::OnSize(PIXEL const width, PIXEL const height)
{
	TimeGraph::OnSize(width, height);
	Trigger();  // cause repaint
	return true;
}

fPixel PreviewControl::getY(fPixel const fPix) const 
{ 
	return Convert2fPixel(GetClientWindowHeight()) - fPix; 
}

fPixel PreviewControl::yVolt(mV const volt) const 
{ 
	return getY(m_vertCoord.Transform2fPixelPos(volt)); 
}

fPixelPoint PreviewControl::pixPntVolt(fMicroSecs const t, mV const v) const 
{ 
	return fPixelPoint(xTime(t), yVolt(v)); 
}

fPixelPoint PreviewControl::pixPntVolt(fMicroSecs const t) const 
{ 
	return pixPntVolt(t, m_pSigGen->GetAmplitude(t)); 
}
