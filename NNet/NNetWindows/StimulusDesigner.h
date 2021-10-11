// StimulusDesigner.h
//
// NNetWindows

#pragma once

#include "D2D_DrawContext.h"
#include "MoreTypes.h"
#include "PixelTypes.h"
#include "win32_baseWindow.h"

class NNetModelReaderInterface;

class StimulusDesigner : public BaseWindow
{
public:
    virtual ~StimulusDesigner() {};

	void Start
	(
		HWND                     const,
		NNetModelReaderInterface const &
    );

	void Reset();
	void Stop();

private:
	TRACKMOUSEEVENT m_trackStruct { sizeof(TRACKMOUSEEVENT), TME_LEAVE, HWND(0), 0L };

	virtual void OnPaint     ();
	virtual bool OnSize      (WPARAM const, LPARAM const);
	virtual void OnMouseWheel(WPARAM const, LPARAM const);

	void         doPaint()                      const;
	fHertz const stimulusFunc(fMicroSecs const) const;

	fPixel     const fHertz2fPixel    (fHertz     const) const;
	fMicroSecs const fPixel2fMicroSecs(fPixel     const) const;
	fPixel     const fMicroSecs2fPixel(fMicroSecs const) const;

	void setMaximum(fMicroSecs const, fHertz const);

	D2D_driver                       m_graphics           { };
	fMicroSecs                       m_fMicroSecsPerPixel { 100.0_MicroSecs };
	fPixel                           m_fPixWinWidth       { 0.0_fPixel };
	fHertz                           m_fHertzPerPixel     { 1.0_fHertz };
	NNetModelReaderInterface const * m_pNMRI              { nullptr };

	float m_A { 1.0f }; // Parameter for stimulus function
	float m_B { 1.0f }; // Parameter for stimulus function
};