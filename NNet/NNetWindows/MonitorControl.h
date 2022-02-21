// MonitorControl.h
//
// NNetWindows

#pragma once

#include "Direct2D.h"
#include "MoreTypes.h"
#include "PixelTypes.h"
#include "PixFpDimension.h"
#include "Measurement.h"
#include "Track.h"
#include "TrackNr.h"
#include "SoundInterface.h"
#include "win32_graphicsWindow.h"

class NNetModelReaderInterface;
class MonitorData;
class NNetModelCommands;

struct IDWriteTextFormat;

class MonitorControl : public GraphicsWindow
{
public:
	MonitorControl
	(
		HWND const, 
		Sound                          &,
		NNetModelCommands              &,
		NNetModelReaderInterface const &,
		MonitorData                    &,
		PixFpDimension<fMicroSecs>     &
	);

	~MonitorControl() final = default;

	void Stop () final;
	void Reset() final;

	bool SignalOverdriven() const;
	void ScaleSignals();

	LPARAM AddContextMenuEntries(HMENU const) final;

	void SetRightBorder(fPixel const fPixBorder)
	{
		m_fPixRightBorder = fPixBorder;
	}

private:

	bool OnCommand        (WPARAM const, LPARAM const, PixelPoint const = PixelPoint::NULL_VAL()) final;
	bool OnMouseLeave     (WPARAM const, LPARAM const) final;
	bool OnShow           (WPARAM const, LPARAM const) final;
	void OnMouseMove      (WPARAM const, LPARAM const) final;
	void OnLButtonUp      (WPARAM const, LPARAM const) final;
	void OnLButtonDblClick(WPARAM const, LPARAM const) final;
	void OnChar           (WPARAM const, LPARAM const) final { /* */ };
	void OnMouseWheel     (WPARAM const, LPARAM const) final;
	bool OnSize           (PIXEL  const, PIXEL  const) final;

	void        moveOperation   (PixelPoint const &);
	void        selectSignal    (PixelPoint const &);
	void        selectTrack     (PixelPoint const &);
	SignalNr    findSignal      (TrackNr const, PixelPoint const &) const;
	TrackNr     findTrack       (PIXEL const) const;
	TrackNr     findPos4NewTrack(PIXEL const) const;
	void        highlightSignal (SignalId const &);
	fPixel      getSignalOffset (SignalId const &) const;
	fPixel      getSignalValue  (Signal const &, fMicroSecs const) const;
	fMicroSecs  findNextMax     (Signal const &, fPixel     const) const;
	fPixel      calcTrackHeight () const;
	fPixelPoint calcDiamondPos  () const;

	void paintSignal(SignalId const &);
	void paintTrack(TrackNr const) const;

	void DoPaint() final;

	inline static HCURSOR m_hCrsrWE { nullptr };
	inline static HCURSOR m_hCrsrNS { nullptr };

	Sound                          & m_sound;        
	NNetModelCommands              & m_modelCommands;
	NNetModelReaderInterface const & m_nmri;  
	MonitorData                    & m_monitorData;

	TrackNr             m_trackNrHighlighted { TrackNr::NULL_VAL() };
	PixelPoint          m_pixLast            { PP_NULL };     // last cursor position during selection 
	PIXEL               m_pixMoveOffsetY     { 0_PIXEL };     // vertical offset when moving signal
	fPixel              m_fPixWinWidth       { 0.0_fPixel };
	fPixel              m_fPixRightBorder    { 0.0_fPixel };
	IDWriteTextFormat * m_pTextFormat        { nullptr };
	float               m_fScaleFactor       { 1.0f };
	fPixel              m_fPixMaxSignal      { 0.0_fPixel };
	Measurement         m_measurement;

	PixFpDimension<fMicroSecs> & m_horzCoord;
	PixFpDimension<float>        m_vertCoord;
};