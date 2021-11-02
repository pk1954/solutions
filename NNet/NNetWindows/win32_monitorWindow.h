// win32_monitorWindow.h : 
//
// NNetWindows

#pragma once

#include "D2D_DrawContext.h"
#include "win32_scale.h"
#include "MoreTypes.h"
#include "Measurement.h"
#include "win32_baseWindow.h"
#include "win32_NNetController.h"

class NNetModelReaderInterface;
class MonitorData;

class MonitorWindow : public BaseWindow
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

	virtual LPARAM AddContextMenuEntries(HMENU const);

private:

	SignalNr   const findSignal       (TrackNr const, PixelPoint const &) const;
	fPixel     const calcTrackHeight  () const;
	TrackNr    const findTrack        (PIXEL const) const;
	TrackNr    const findPos4NewTrack (PIXEL const) const;
	fPixel     const getSignalOffset  (SignalId   const &) const;
	fPixel     const getYvalue        (Signal const &, fMicroSecs const) const;
	fMicroSecs const findNextMax      (Signal const &, fPixel     const) const;
	void             highlightSignal  (SignalId const &);

	void doPaint() const;
	void paintSignal(SignalId const &) const;
	void drawDiamond() const;

	virtual void OnPaint();
	virtual bool OnSize              (WPARAM const, LPARAM const);
	virtual void OnMouseWheel        (WPARAM const, LPARAM const);
	virtual void OnMouseMove         (WPARAM const, LPARAM const);
	virtual bool OnMouseLeave        (WPARAM const, LPARAM const);
	virtual void OnLButtonUp         (WPARAM const, LPARAM const);
	virtual void OnLeftButtonDblClick(WPARAM const, LPARAM const);
	virtual bool OnShow              (WPARAM const, LPARAM const);
	virtual bool OnCommand           (WPARAM const, LPARAM const, PixelPoint const = PixelPoint::NULL_VAL());
	virtual void OnChar              (WPARAM const, LPARAM const) { };

	inline static HCURSOR m_hCrsrWE { nullptr };
	inline static HCURSOR m_hCrsrNS { nullptr };

	Sound                          * m_pSound         { nullptr };
	NNetController                 * m_pController    { nullptr };
	NNetModelCommands              * m_pModelCommands { nullptr };
	NNetModelReaderInterface const * m_pNMRI          { nullptr };
	MonitorData                    * m_pMonitorData   { nullptr };  

	TRACKMOUSEEVENT m_trackStruct { sizeof(TRACKMOUSEEVENT), TME_LEAVE, HWND(0), 0L };

	D2D_driver   m_graphics           { };
	TrackNr      m_trackNrHighlighted { TrackNr::NULL_VAL() };
	PixelPoint   m_pixLast            { PP_NULL };     // last cursor position during selection 
	PIXEL        m_pixMoveOffsetY     { 0_PIXEL };     // vertical offset when moving signal
	fPixel       m_fPixWinWidth       { 0.0_fPixel };
	bool         m_bShowScale         { false };
	Measurement  m_measurement;

	Scale<fMicroSecs>      m_horzScale;
	PixCoordFp<fMicroSecs> m_horzCoord;
	PixCoordFp<float>      m_vertCoord;
};
