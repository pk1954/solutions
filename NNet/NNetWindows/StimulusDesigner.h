// StimulusDesigner.h
//
// NNetWindows

#pragma once

#include "D2D_DrawContext.h"
#include "Scale.h"
#include "MoreTypes.h"
#include "PixelTypes.h"
#include "win32_baseWindow.h"

class SignalGenerator;
class NNetModelReaderInterface;

class StimulusDesigner : public BaseWindow
{
public:
    virtual ~StimulusDesigner() {};

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
	virtual void OnMouseWheel(WPARAM const, LPARAM const);

	fPixel      const fHertz2fPixel    (fHertz     const) const;
	fMicroSecs  const fPixel2fMicroSecs(fPixel     const) const;
	fPixel      const fMicroSecs2fPixel(fMicroSecs const) const;
	fPixelPoint const getPixPnt        (fMicroSecs const) const;

	TRACKMOUSEEVENT m_trackStruct { sizeof(TRACKMOUSEEVENT), TME_LEAVE, HWND(0), 0L };

	D2D_driver                       m_graphics           { };
	fMicroSecs                       m_fMicroSecsPerPixel { 100.0_MicroSecs };
	fPixelPoint                      m_fPixOffset         { fPP_NULL };
	fPixel                           m_fPixGraphWidth     { 0.0_fPixel };
	fHertz                           m_fHertzPerPixel     { 0.25_fHertz };
	NNetModelReaderInterface const * m_pNMRI              { nullptr };
	SignalGenerator                * m_pSignalGenerator   { nullptr };
	Scale                            m_scale;
};