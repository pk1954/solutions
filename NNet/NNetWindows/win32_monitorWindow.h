// win32_monitorWindow.h : 
//
// NNetWindows

#pragma once

#include "win32_scale.h"
#include "MoreTypes.h"
#include "Measurement.h"
#include "win32_graphicsWindow.h"
#include "win32_NNetController.h"

class NNetModelReaderInterface;
class MonitorData;

class MonitorWindow : public GraphicsWindow
{
public:

	void Start
	(
		HWND                     const, 
		Sound                  * const,
		NNetController         * const,
		NNetModelCommands      * const,
		NNetModelReaderInterface const &, 
		MonitorData                    &
	);

	void Reset();
	void Stop();

	LPARAM AddContextMenuEntries(HMENU const) final;

private:

	void       moveOperation    (PixelPoint const &);
	void       selectSignal     (PixelPoint const &);
	void       selectTrack      (PixelPoint const &);
	SignalNr   findSignal       (TrackNr const, PixelPoint const &) const;
	fPixel     calcTrackHeight  () const;
	TrackNr    findTrack        (PIXEL const) const;
	TrackNr    findPos4NewTrack (PIXEL const) const;
	fPixel     getSignalOffset  (SignalId   const &) const;
	fPixel     getSignalValue   (Signal const &, fMicroSecs const) const;
	fMicroSecs findNextMax      (Signal const &, fPixel     const) const;
	void       highlightSignal  (SignalId const &);

	void paintSignal(SignalId const &) const;

	fPixelPoint calcDiamondPos() const;

	void DoPaint() final;

	bool OnSize              (WPARAM const, LPARAM const) final;
	void OnMouseWheel        (WPARAM const, LPARAM const) final;
	void OnMouseMove         (WPARAM const, LPARAM const) final;
	bool OnMouseLeave        (WPARAM const, LPARAM const) final;
	void OnLButtonUp         (WPARAM const, LPARAM const) final;
	void OnLeftButtonDblClick(WPARAM const, LPARAM const) final;
	bool OnShow              (WPARAM const, LPARAM const) final;
	bool OnCommand           (WPARAM const, LPARAM const, PixelPoint const = PixelPoint::NULL_VAL()) final;
	void OnChar              (WPARAM const, LPARAM const) final { };

	inline static HCURSOR m_hCrsrWE { nullptr };
	inline static HCURSOR m_hCrsrNS { nullptr };

	Sound                          * m_pSound         { nullptr };
	NNetController                 * m_pController    { nullptr };
	NNetModelCommands              * m_pModelCommands { nullptr };
	NNetModelReaderInterface const * m_pNMRI          { nullptr };
	MonitorData                    * m_pMonitorData   { nullptr };  


	TrackNr      m_trackNrHighlighted { TrackNr::NULL_VAL() };
	PixelPoint   m_pixLast            { PP_NULL };     // last cursor position during selection 
	PIXEL        m_pixMoveOffsetY     { 0_PIXEL };     // vertical offset when moving signal
	fPixel       m_fPixWinWidth       { 0.0_fPixel };
	Measurement  m_measurement;

	PixCoordFp<fMicroSecs> m_horzCoord;
	PixCoordFp<float>      m_vertCoord;
};
