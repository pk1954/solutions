// StimulusDesigner.h
//
// NNetWindows

#pragma once

#include "D2D_DrawContext.h"
#include "win32_scale.h"
#include "MoreTypes.h"
#include "PixelTypes.h"
#include "PixCoordFp.h"
#include "win32_baseWindow.h"

class SignalGenerator;
class NNetModelReaderInterface;

class StimulusDesigner : public BaseWindow
{
public:
    virtual ~StimulusDesigner() {};

	static void Initialize(Param & param) 
	{ 
		m_pParameters = & param; 
	}

	void Start
	(
		HWND                     const,
		SignalGenerator        * const,
		NNetModelReaderInterface const &
	);

	void Reset();
	void Stop();

private:

	void doPaint() const;

	virtual void OnPaint     ();
	virtual bool OnSize      (WPARAM const, LPARAM const);
	virtual void OnMouseMove (WPARAM const, LPARAM const);
	virtual void OnMouseWheel(WPARAM const, LPARAM const);

	fPixel      const getPixX    (fMicroSecs const) const;
	fPixel      const getPixY    (fHertz     const) const;
	fPixelPoint const getPixPnt  (fMicroSecs const, fHertz const) const;
	fPixelPoint const getGraphPnt(fMicroSecs const)               const;

	bool horzScaleSelected  (fPixelPoint const &) const;
	bool vertScaleSelected  (fPixelPoint const &) const;
	bool baseLineSelected   (fPixelPoint const &) const;
	bool freqMaxLineSelected(fPixelPoint const &) const;
	bool timeMaxLineSelected(fPixelPoint const &) const;

	void displayBaseFrequency   (fMicroSecs const, fMicroSecs const, bool const) const;
	void displayFreqMaxLine     (bool const) const;
	void displayTimeMaxLine     (bool const) const;
	void displayDiamondAtMaximum(bool const) const;

	enum class tTrackMode { NONE, MAX_PNT, MAX_FREQ, MAX_TIME, BASE_FREQ };
	enum class tZoomMode  { NONE, HORZ, VERT };

	TRACKMOUSEEVENT m_trackStruct { sizeof(TRACKMOUSEEVENT), TME_LEAVE, HWND(0), 0L };

	inline static Param * m_pParameters { nullptr };

	D2D_driver                       m_graphics         { };
	tTrackMode                       m_trackMode        { tTrackMode::NONE };
	tZoomMode                        m_zoomMode         { tZoomMode::NONE };
	fPixel                           m_fPixGraphWidth   { 0.0_fPixel };
	fPixel                           m_fPixLineWidth    { 1.0_fPixel };
	fPixelPoint                      m_fPixPntMax       { };
	NNetModelReaderInterface const * m_pNMRI            { nullptr };
	SignalGenerator                * m_pSignalGenerator { nullptr };
	Scale<fMicroSecs>                m_horzScale;
	Scale<fHertz>                    m_vertScale;
	PixCoordFp<fMicroSecs>           m_horzCoord;
	PixCoordFp<fHertz>               m_vertCoord;

	inline static fPixel const DIAMOND_SIZE { 4.0_fPixel };
};