// SignalControl.h
//
// NNetWindows

#pragma once

#include <array>
#include "Direct2D.h"
#include "MoreTypes.h"
#include "PixelTypes.h"
#include "PixFpDimension.h"
#include "SignalGenerator.h"
#include "NNetParameters.h"
#include "win32_graphicsWindow.h"

using std::array;

class ComputeThread;
class NNetModelCommands;
class NNetModelWriterInterface;

class SignalControl : public GraphicsWindow
{
public:
	SignalControl
	(
		HWND                 const, 
		ComputeThread        const &, 
		NNetModelCommands          &,
		SignalGenerator            &,
		PixFpDimension<fMicroSecs> &,
		PixFpDimension<fHertz>     &,
		PixFpDimension<mV>         &
	);

	~SignalControl() final;

	static void Initialize(Param & param) 
	{ 
		m_pParameters = & param; 
	}

	enum class tColor { FREQ, VOLT, TIME, HIGH };

	void SetColor(tColor const type, D2D1::ColorF const col)
	{
		m_colTable[static_cast<int>(type)] = col;
	}

private:

	void paintCurve(auto getPoint, D2D1::ColorF const col) const
	{
		fMicroSecs const usResolution { m_sigGen.GetParams().GetParameterValue(ParamType::Value::timeResolution) };
		fMicroSecs const usPixelSize  { m_horzCoord.GetPixelSize() };
		fMicroSecs const usIncrement  { (usPixelSize > usResolution) ? usPixelSize : usResolution };
		fMicroSecs const timeStart    { 0.0_MicroSecs };
		fMicroSecs const timeCutoff   { m_sigGen.CutoffTime() };
		fMicroSecs       timeEnd      { m_horzCoord.Transform2logUnitSize(m_fPixGraphWidth) };
		fPixelPoint      prevPoint    { getPoint(timeStart) };

		if (timeCutoff < timeEnd)
			timeEnd = timeCutoff;

		for (fMicroSecs time = timeStart + usIncrement; time < timeEnd; time += usIncrement)
		{
			fPixelPoint const actPoint  { getPoint(time) };
			fPixelPoint const stepPoint { actPoint.GetX(), prevPoint.GetY() };
			m_upGraphics->DrawLine(prevPoint, stepPoint, m_fPixLineWidth, col);
			m_upGraphics->DrawLine(stepPoint, actPoint,  m_fPixLineWidth, col);
			prevPoint = actPoint;
		}
	}

	void DoPaint() final;

	bool OnSize      (PIXEL  const, PIXEL  const) final;
	void OnMouseMove (WPARAM const, LPARAM const) final;
	bool OnMouseLeave(WPARAM const, LPARAM const) final;

	fPixel     getY   (fPixel const fPix) const { return yBottom() - fPix; }
	fMicroSecs getTime(fPixel const fPix) const { return m_horzCoord.Transform2logUnitPos(fPix); }
	fHertz     getFreq(fPixel const fPix) const { return m_vertCoordFreq.Transform2logUnitPos(getY(fPix)); }
	mV         getVolt(fPixel const fPix) const { return m_vertCoordVolt.Transform2logUnitPos(getY(fPix)); }

	fPixel xTime(fMicroSecs const time) const { return fPixel(m_horzCoord.Transform2fPixelPos(time)); }
	fPixel yPos (fPixel     const fPix) const { return yBottom() - fPix; }
	fPixel yFreq(fHertz     const freq) const { return yPos(m_vertCoordFreq.Transform2fPixelPos(freq)); }
	fPixel yVolt(mV         const volt) const { return yPos(m_vertCoordVolt.Transform2fPixelPos(volt)); }

	fPixel xLeft  () const { return Convert2fPixel(0_PIXEL); }
	fPixel xRight () const { return Convert2fPixel(GetClientWindowWidth());  }
	fPixel yBottom() const { return Convert2fPixel(GetClientWindowHeight()); }
	fPixel xPeak  () const { return xTime(m_sigGen.TimePeak());	}

	fPixel yPeakVolt() const { return yVolt(m_sigGen.Voltage().peak); }
	fPixel yBaseVolt() const { return yVolt(m_sigGen.Voltage().base); }
	fPixel yPeakFreq() const { return yFreq(m_sigGen.Frequency().peak); }
	fPixel yBaseFreq() const { return yFreq(m_sigGen.Frequency().base); }

	fPixelPoint pixPntFreq(fMicroSecs const t, fHertz const f) const { return fPixelPoint(xTime(t), yFreq(f)); }
	fPixelPoint pixPntVolt(fMicroSecs const t, mV     const v) const { return fPixelPoint(xTime(t), yVolt(v)); }
	fPixelPoint pixPntFreq(fMicroSecs const t) const { return pixPntFreq(t, m_sigGen.GetFrequency(t)); }
	fPixelPoint pixPntVolt(fMicroSecs const t) const { return pixPntVolt(t, m_sigGen.GetVoltage(t)); }

	fPixelPoint handleBaseFreq() const { return fPixelPoint(xLeft (), yBaseFreq()); }
	fPixelPoint handlePeakFreq() const { return fPixelPoint(xLeft (), yPeakFreq()); }
	fPixelPoint handleTimeFreq() const { return fPixelPoint(xPeak (), yPeakFreq()); }
	fPixelPoint handleBaseVolt() const { return fPixelPoint(xRight(), yBaseVolt()); }
	fPixelPoint handlePeakVolt() const { return fPixelPoint(xRight(), yPeakVolt()); }
	fPixelPoint handleTimeVolt() const { return fPixelPoint(xPeak (), yPeakVolt()); }
	fPixelPoint handlePeakTime() const { return fPixelPoint(xPeak (), yBottom  ()); }

	void drawLine(tColor, fPixelPoint const &, fPixelPoint const &) const;
	void drawDiam(tColor, fPixelPoint const &) const;
	void highlightMovedObject() const;

	void setMoveMode(fPixelPoint const &);

	enum class tMoveMode  { NONE, TIME, BASE_FREQ, PEAK_FREQ, TIME_FREQ, BASE_VOLT, PEAK_VOLT, TIME_VOLT };

	array<D2D1::ColorF, 4> m_colTable 
	{ 
		D2D1::ColorF::Black, // FREQ
		D2D1::ColorF::Black, // VOLT
		D2D1::ColorF::Black, // TIME
		D2D1::ColorF::Red    // HIGH
	};

	D2D1::ColorF getColor(tColor const type) const
	{
		return m_colTable[static_cast<int>(type)];
	}

	inline static Param * m_pParameters { nullptr };

	PixFpDimension<fMicroSecs> & m_horzCoord;
	PixFpDimension<fHertz>     & m_vertCoordFreq;
	PixFpDimension<mV>         & m_vertCoordVolt;
	ComputeThread        const & m_computeThread;
	SignalGenerator            & m_sigGen;
	NNetModelCommands          & m_commands;
	SignalGenerator              m_sigGenNew;
	tMoveMode                    m_moveMode       { tMoveMode::NONE };
	fPixel                       m_fPixGraphWidth { 0.0_fPixel };
	fPixel                       m_fPixLineWidth  { 1.0_fPixel };
	D2D1::ColorF                 m_colorFreq      { D2D1::ColorF::Black };
	D2D1::ColorF                 m_colorVolt      { D2D1::ColorF::Black };
	D2D1::ColorF                 m_colorTime      { D2D1::ColorF::Black };
	D2D1::ColorF                 m_colorHigh      { D2D1::ColorF::Red };
};