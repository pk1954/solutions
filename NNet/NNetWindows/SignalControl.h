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
#include "TimeGraph.h"

using std::array;

class ComputeThread;
class NNetModelCommands;
class NNetModelWriterInterface;

class SignalControl : public TimeGraph
{
public:
	SignalControl
	(
		HWND                 const, 
		ComputeThread        const &, 
		NNetModelCommands          &,
		SignalGenerator            * const,
		PixFpDimension<fMicroSecs> *
	);

	~SignalControl() final;

	enum class tColor { FREQ, VOLT, TIME, HIGH };

	void SetColor(tColor const type, D2D1::ColorF const col)
	{
		m_colTable[static_cast<int>(type)] = col;
	}

	void SetVertCoordFreq(PixFpDimension<fHertz> *);
	void SetVertCoordVolt(PixFpDimension<mV>     *);

	void ScaleTimeCoord();
	void ScaleFreqCoord();
	void ScaleVoltCoord();

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

	fPixel const STD_DIAMOND  { 5.0_fPixel };
	fPixel const HIGH_DIAMOND { 8.0_fPixel };

	PixFpDimension<fHertz> * m_pVertCoordFreq { nullptr };
	PixFpDimension<mV>     * m_pVertCoordVolt { nullptr };
	ComputeThread    const & m_computeThread;
	NNetModelCommands      & m_commands;
	tPos                     m_moveMode { tPos::NONE };

	void DoPaint() final;

	bool OnSize      (PIXEL  const, PIXEL  const) final;
	void OnMouseMove (WPARAM const, LPARAM const) final;
	bool OnMouseLeave(WPARAM const, LPARAM const) final;

	fPixel getY(fPixel const fPix) const { return yBottom() - fPix; }

	fHertz getFreq(fPixelPoint const & p) const { return m_pVertCoordFreq->Transform2logUnitPos(getY(p.GetY())); }
	mV     getVolt(fPixelPoint const & p) const { return m_pVertCoordVolt->Transform2logUnitPos(getY(p.GetY())); }

	fPixel yFreq(fHertz     const freq) const { return getY(m_pVertCoordFreq->Transform2fPixelPos(freq)); }
	fPixel yVolt(mV         const volt) const { return getY(m_pVertCoordVolt->Transform2fPixelPos(volt)); }

	fPixel xPeak    () const { return xTime(m_pSigGen->TimePeak());	}
	fPixel yPeakVolt() const { return yVolt(m_pSigGen->Voltage().Peak()); }
	fPixel yBaseVolt() const { return yVolt(m_pSigGen->Voltage().Base()); }
	fPixel yPeakFreq() const { return yFreq(m_pSigGen->Frequency().Peak()); }
	fPixel yBaseFreq() const { return yFreq(m_pSigGen->Frequency().Base()); }

	fPixelPoint pixPntFreq(fMicroSecs const t, fHertz const f) const { return fPixelPoint(xTime(t), yFreq(f)); }
	fPixelPoint pixPntVolt(fMicroSecs const t, mV     const v) const { return fPixelPoint(xTime(t), yVolt(v)); }
	fPixelPoint pixPntFreq(fMicroSecs const t) const { return pixPntFreq(t, m_pSigGen->GetFrequency(t)); }
	fPixelPoint pixPntVolt(fMicroSecs const t) const { return pixPntVolt(t, m_pSigGen->GetVoltage(t)); }

	void calcHandles();
	void paintRunControls () const;
	void paintEditControls() const;
	void drawLine(tColor, tPos const, tPos const) const;
	void drawDiam(tColor, tPos const) const;
	void highlightMovedObject() const;
	void testPos(tPos const, fPixelPoint const, fPixel &);
	void setPos(fPixelPoint const &);
};