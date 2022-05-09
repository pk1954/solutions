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

class MonitorData;
class NNetModelCommands;
class NNetModelWriterInterface;

struct IDWriteTextFormat;

class MonitorControl : public GraphicsWindow
{
public:
	MonitorControl
	(
		HWND const, 
		Sound                      &,
		NNetModelCommands          &,
		PixFpDimension<fMicroSecs> &,
		PixFpDimension<float> &
	);

	~MonitorControl() final = default;

	void Stop () final;
	void Reset();
	void SetModelInterface(NNetModelWriterInterface * const);
	bool SignalTooHigh() const;
	void ScaleSignals();

	LPARAM AddContextMenuEntries(HMENU const) final;

	void SetRightBorder(fPixel const fPixBorder)
	{
		m_fPixRightBorder = fPixBorder;
	}

private:

	bool OnCommand        (WPARAM const, LPARAM const, PixelPoint const = PixelPoint::NULL_VAL()) final;
	void OnMouseLeave     () final;
	bool OnShow           (WPARAM const, LPARAM const) final;
	void OnMouseMove      (WPARAM const, LPARAM const) final;
	bool OnLButtonUp      (WPARAM const, LPARAM const) final;
	void OnLButtonDblClick(WPARAM const, LPARAM const) final;
	void OnChar           (WPARAM const, LPARAM const) final { /* */ };
	void OnMouseWheel     (WPARAM const, LPARAM const) final;
	bool OnSize           (PIXEL  const, PIXEL  const) final;

	void        moveOperation   (PixelPoint const &);
	void        selectSignal    (PixelPoint const &);
	void        selectTrack     (PixelPoint const &);
	SignalNr    findSignal      (TrackNr const, PixelPoint const &, bool const) const;
	TrackNr     findTrack       (PIXEL const) const;
	TrackNr     findPos4NewTrack(PIXEL const) const;
	void        highlightSignal (SignalId const &);
	fPixel      getSignalOffset (SignalId const &) const;
	fPixel      getSignalValue  (Signal const &, fMicroSecs const, bool const) const;
	fPixel      calcTrackHeight () const;
	fPixelPoint calcDiamondPos  (bool const) const;

	void paintSignal(SignalId const &, bool const);
	void paintTrack(TrackNr const) const;

	void DoPaint() final;

	inline static HCURSOR m_hCrsrWE { nullptr };
	inline static HCURSOR m_hCrsrNS { nullptr };

	Sound                    & m_sound;        
	NNetModelCommands        & m_modelCommands;
	MonitorData              * m_pMonitorData { nullptr };
	NNetModelWriterInterface * m_pNMWI        { nullptr };

	TrackNr             m_trackNrHighlighted { TrackNr::NULL_VAL() };
	PixelPoint          m_pixLast            { PP_NULL };     // last cursor position during selection 
	PIXEL               m_pixMoveOffsetY     { 0_PIXEL };     // vertical offset when moving signal
	fPixel              m_fPixWinWidth       { 0.0_fPixel };
	fPixel              m_fPixRightBorder    { 0.0_fPixel };
	fPixel              m_fPixZeroX          { 0.0_fPixel };
	IDWriteTextFormat * m_pTextFormat        { nullptr };
	fPixel              m_fPixMaxSignal      { 0.0_fPixel };
	Measurement         m_measurement;

	PixFpDimension<fMicroSecs> & m_horzCoord;
	PixFpDimension<float>      & m_vertCoord;
};