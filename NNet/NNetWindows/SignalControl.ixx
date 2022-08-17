// SignalControl.ixx
//
// NNetWindows

module;

#include <array>
#include "SignalGenerator.h"
#include "NNetModelCommands.h"
#include "NNetModelReaderInterface.h"

export module SignalControl;

import MoreTypes;
import VoltageType;
import PixFpDimension;
import ComputeThread;
import Direct2D;
import PixelTypes;
import TimeGraph;

using std::array;
using D2D1::ColorF;

export class SignalControl : public TimeGraph
{
public:
	SignalControl
	(
		HWND                 const, 
		ComputeThread        const &, 
		NNetModelCommands          &,
		Observable                 &,
		Observable                 &,
		PixFpDimension<fMicroSecs> *
	);

	~SignalControl() final;

	enum class tColor { FREQ, VOLT, TIME, HIGH };

	void SetColor(tColor const type, ColorF const col)
	{
		m_colTable[static_cast<int>(type)] = col;
	}

	void SetVertCoordFreq(PixFpDimension<fHertz> *);
	void SetVertCoordVolt(PixFpDimension<mV>     *);

	float ScaleFactorTimeCoord() const;
	float ScaleFactorFreqCoord() const;
	float ScaleFactorVoltCoord() const;

private:

	enum class tPos { NONE, TIME, BASE_FREQ, PEAK_FREQ, TIME_FREQ, BASE_VOLT, PEAK_VOLT, TIME_VOLT, BASA_FREQ, BASA_VOLT };

	array<fPixelPoint,10> m_handles;

	fPixelPoint getPos(tPos const mode) const 
	{ 
		return m_handles[static_cast<int>(mode)]; 
	}

	array<ColorF, 4> m_colTable 
	{ 
		ColorF::Black, // FREQ
		ColorF::Black, // VOLT
		ColorF::Black, // TIME
		ColorF::Red    // HIGH
	};

	ColorF getColor(tColor const type) const 
	{
		return m_colTable[static_cast<int>(type)];
	}

	fPixel const STD_DIAMOND  { 5.0_fPixel };
	fPixel const HIGH_DIAMOND { 8.0_fPixel };

	PixFpDimension<fHertz> * m_pVertCoordFreq { nullptr };
	PixFpDimension<mV>     * m_pVertCoordVolt { nullptr };
	ComputeThread    const & m_computeThread;
	NNetModelCommands      & m_commands;
	Observable             & m_runObservable;
	Observable             & m_dynamicModelObservable;
	tPos                     m_moveMode { tPos::NONE };

	void DoPaint() final;

	void OnMouseMove  (WPARAM const, LPARAM const) final;
	bool OnLButtonDown(WPARAM const, LPARAM const) final;
	bool OnLButtonUp  (WPARAM const, LPARAM const) final;
	void OnMouseLeave () final;

	fPixel getY(fPixel const fPix) const { return yBottom() - fPix; }

	fHertz getFreq(fPixel      const fPixY) const { return m_pVertCoordFreq->Transform2logUnitPos(getY(fPixY)); }
	fHertz getFreq(fPixelPoint const & p  ) const { return getFreq(p.GetY()); }
	mV     getVolt(fPixel      const fPixY) const { return m_pVertCoordVolt->Transform2logUnitPos(getY(fPixY)); }
	mV     getVolt(fPixelPoint const & p  ) const { return getVolt(p.GetY()); }

	fPixel yFreq(fHertz const freq) const { return getY(m_pVertCoordFreq->Transform2fPixelPos(freq)); }
	fPixel yVolt(mV     const volt) const { return getY(m_pVertCoordVolt->Transform2fPixelPos(volt)); }

	fPixel xPeak      () const { return xTime(GetSigGenStaticData()->GetPeakTime()); }
	fPixel aPeakAmplit() const { return yVolt(GetSigGenStaticData()->GetAmplitude().Peak()); }
	fPixel yBaseAmplit() const { return yVolt(GetSigGenStaticData()->GetAmplitude().Base()); }
	fPixel yPeakFreq  () const { return yFreq(GetSigGenStaticData()->GetFrequency().Peak()); }
	fPixel yBaseFreq  () const { return yFreq(GetSigGenStaticData()->GetFrequency().Base()); }

	fPixelPoint pixPntFreq(fMicroSecs const t, fHertz const f) const { return fPixelPoint(xTime(t), yFreq(f)); }
	fPixelPoint pixPntVolt(fMicroSecs const t, mV     const v) const { return fPixelPoint(xTime(t), yVolt(v)); }
	fPixelPoint pixPntStimulusFreq(fMicroSecs const t) const { return pixPntFreq(t, GetSigGenSelected()->GetStimulusFrequency(t)); }
	fPixelPoint pixPntStimulusVolt(fMicroSecs const t) const { return pixPntVolt(t, GetSigGenSelected()->GetStimulusAmplitude(t)); }

	void calcHandles();
	void paintRunControls (fMicroSecs const) const;
	void paintEditControls() const;
	void drawLine(tColor, tPos const, tPos const) const;
	void drawDiam(tColor, tPos const) const;
	void highlightMovedObject() const;
	void testPos(tPos const, fPixelPoint const, fPixel &);
	void setPos(fPixelPoint const &);
};