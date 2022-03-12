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
		PixFpDimension<fHertz>     *,
		PixFpDimension<mV>         *
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

	enum class tPos { NONE, TIME, BASE_FREQ, PEAK_FREQ, TIME_FREQ, BASE_VOLT, PEAK_VOLT, TIME_VOLT, BASA_FREQ, BASA_VOLT };

	array<fPixelPoint,10> m_handles;

	fPixelPoint getPos(tPos const mode) const 
	{ 
		return m_handles[static_cast<int>(mode)]; 
	}

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

	fPixel const STD_WIDTH  { 1.0_fPixel };
	fPixel const HIGH_WIDTH { 3.0_fPixel };

	fPixel const STD_DIAMOND  { 5.0_fPixel };
	fPixel const HIGH_DIAMOND { 8.0_fPixel };

	PixFpDimension<fMicroSecs> & m_horzCoord;
	PixFpDimension<fHertz>     * m_pVertCoordFreq;
	PixFpDimension<mV>         * m_pVertCoordVolt;
	ComputeThread        const & m_computeThread;
	SignalGenerator            & m_sigGen;
	NNetModelCommands          & m_commands;
	SignalGenerator              m_sigGenNew;
	tPos                         m_moveMode   { tPos::NONE };
	fPixel                       m_fPixLeft   { 0.0_fPixel };
	fPixel                       m_fPixRight  { 0.0_fPixel };
	fPixel                       m_fPixBottom { 0.0_fPixel };

	void paintCurve(auto getPoint, tColor const colType) const
	{
		D2D1::ColorF const col          { getColor(colType) };
		fMicroSecs   const usResolution { m_sigGen.GetParams().GetParameterValue(ParamType::Value::timeResolution) };
		fMicroSecs   const usPixelSize  { m_horzCoord.GetPixelSize() };
		fMicroSecs   const usIncrement  { max(usPixelSize, usResolution) };
		fMicroSecs   const timeStart    { 0.0_MicroSecs };
		fMicroSecs   const timeEnd      { min(getTime(m_fPixRight), m_sigGen.CutoffTime()) };
		fPixelPoint        prevPoint    { getPoint(timeStart) };

		for (fMicroSecs time = timeStart + usIncrement; time < timeEnd; time += usIncrement)
		{
			fPixelPoint const actPoint  { getPoint(time) };
			fPixelPoint const stepPoint { actPoint.GetX(), prevPoint.GetY() };
			m_upGraphics->DrawLine(prevPoint, stepPoint, STD_WIDTH, col);
			m_upGraphics->DrawLine(stepPoint, actPoint,  STD_WIDTH, col);
			prevPoint = actPoint;
		}
	}

	void DoPaint() final;

	bool OnSize      (PIXEL  const, PIXEL  const) final;
	void OnMouseMove (WPARAM const, LPARAM const) final;
	bool OnMouseLeave(WPARAM const, LPARAM const) final;

	fPixel xLeft  () const { return m_fPixLeft;   }
	fPixel xRight () const { return m_fPixRight;  }
	fPixel yBottom() const { return m_fPixBottom; }
	fPixel xPeak  () const { return xTime(m_sigGen.TimePeak());	}

	fPixel     getY   (fPixel const fPix) const { return m_fPixBottom - fPix; }
	fMicroSecs getTime(fPixel const fPix) const { return m_horzCoord    .Transform2logUnitPos(fPix); }
	fHertz     getFreq(fPixel const fPix) const { return m_pVertCoordFreq->Transform2logUnitPos(getY(fPix)); }
	mV         getVolt(fPixel const fPix) const { return m_pVertCoordVolt->Transform2logUnitPos(getY(fPix)); }

	fPixel xTime(fMicroSecs const time) const { return fPixel(m_horzCoord.Transform2fPixelPos(time)); }
	fPixel yFreq(fHertz     const freq) const { return getY(m_pVertCoordFreq->Transform2fPixelPos(freq)); }
	fPixel yVolt(mV         const volt) const { return getY(m_pVertCoordVolt->Transform2fPixelPos(volt)); }

	fPixel yPeakVolt() const { return yVolt(m_sigGen.Voltage().peak); }
	fPixel yBaseVolt() const { return yVolt(m_sigGen.Voltage().base); }
	fPixel yPeakFreq() const { return yFreq(m_sigGen.Frequency().peak); }
	fPixel yBaseFreq() const { return yFreq(m_sigGen.Frequency().base); }

	fPixelPoint pixPntFreq(fMicroSecs const t, fHertz const f) const { return fPixelPoint(xTime(t), yFreq(f)); }
	fPixelPoint pixPntVolt(fMicroSecs const t, mV     const v) const { return fPixelPoint(xTime(t), yVolt(v)); }
	fPixelPoint pixPntFreq(fMicroSecs const t) const { return pixPntFreq(t, m_sigGen.GetFrequency(t)); }
	fPixelPoint pixPntVolt(fMicroSecs const t) const { return pixPntVolt(t, m_sigGen.GetVoltage(t)); }

	void calcHandles();
	void paintRunControls () const;
	void paintEditControls() const;
	void drawLine(tColor, tPos const, tPos const) const;
	void drawDiam(tColor, tPos const) const;
	void highlightMovedObject() const;
	void testPos(tPos const, fPixelPoint const, fPixel &);
	void setPos(fPixelPoint const &);
};