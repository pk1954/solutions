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

	fPixelPoint const getPixPnt (fMicroSecs const) const;

	TRACKMOUSEEVENT m_trackStruct { sizeof(TRACKMOUSEEVENT), TME_LEAVE, HWND(0), 0L };

	D2D_driver                       m_graphics         { };
	fPixel                           m_fPixGraphWidth   { 0.0_fPixel };
	NNetModelReaderInterface const * m_pNMRI            { nullptr };
	SignalGenerator                * m_pSignalGenerator { nullptr };
	Scale<fMicroSecs>                m_horzScale;
	Scale<fHertz>                    m_vertScale;
	PixCoordFp<fMicroSecs>           m_horzCoord;
	PixCoordFp<fHertz>               m_vertCoord;
};