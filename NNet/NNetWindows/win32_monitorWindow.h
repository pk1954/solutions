// win32_monitorWindow.h : 
//
// NNetWindows

#pragma once

#include <vector>
#include "D2D_DrawContext.h"
#include "Scale.h"
#include "Measurement.h"
#include "MonitorData.h"
#include "win32_baseWindow.h"
#include "win32_NNetController.h"

class Param;
class BeaconAnimation;
class NNetModelReaderInterface;

class MonitorWindow : public BaseWindow
{
public:

	void Start
	( 
		HWND                     const, 
		NNetController         * const,
		NNetModelReaderInterface const &, 
		Param                    const &,
		BeaconAnimation                &,
		MonitorData                    &
	);

	void Reset( );
	void Stop( );

	virtual long AddContextMenuEntries( HMENU const );

	void AddSignal( ShapeId const );
	void InsertTrack( TrackNr const );

private:

	SignalNr const findSignal      ( TrackNr const, PixelPoint const & ) const;
	fPIXEL   const calcTrackHeight ( ) const;
	fPIXEL   const getYvalue       ( Signal const &, fMicroSecs const ) const;
	TrackNr  const findTrack       ( PIXEL const ) const;
	TrackNr  const findPos4NewTrack( PIXEL const ) const;

	void doPaint( ) const;
	void selectSignal( SignalId const & );
	void deselectSignal( );
	void paintSignal ( SignalId const &, fPIXEL const, fPIXEL const, fMicroSecs const, fMicroSecs const ) const;

	virtual void OnPaint( );
	virtual bool OnSize       ( WPARAM const, LPARAM const );
	virtual bool OnSetCursor  ( WPARAM const, LPARAM const );
	virtual void OnMouseWheel ( WPARAM const, LPARAM const );
	virtual void OnMouseMove  ( WPARAM const, LPARAM const );
	virtual bool OnMouseLeave ( WPARAM const, LPARAM const );
	virtual void OnLButtonUp  ( WPARAM const, LPARAM const );
	virtual bool OnShow       ( WPARAM const, LPARAM const );
	virtual bool OnCommand    ( WPARAM const, LPARAM const, PixelPoint const = PixelPoint::NULL_VAL() );
	virtual void OnChar       ( WPARAM const, LPARAM const ) { };

	inline static HCURSOR m_hCrsrWE { nullptr };
	inline static HCURSOR m_hCrsrNS { nullptr };

	NNetController                 * m_pController      { nullptr };
	Param                    const * m_pParams          { nullptr };
	NNetModelReaderInterface const * m_pMRI             { nullptr };
	BeaconAnimation                * m_pBeaconAnimation { nullptr };
	MonitorData                    * m_pMonitorData     { nullptr };  

	TRACKMOUSEEVENT m_trackStruct { sizeof(TRACKMOUSEEVENT), TME_LEAVE, HWND(0), 0L };

	D2D_driver  m_graphics           { };
	fMicroSecs  m_fMicroSecsPerPixel { 100.0_MicroSecs };
	float       m_fYvaluesPerPixel   { 0.2f };
	SignalId    m_idSigSelected      { };
	TrackNr     m_trackNrHighlighted { TrackNr::NULL_VAL() };
	PixelPoint  m_pixLast            { PP_NULL };     // last cursor position during selection 
	PIXEL       m_pixMoveOffsetY     { 0_PIXEL };     // vertical offset when moving signal
	bool        m_bShowScale         { false };
	Measurement m_measurement;
	Scale       m_scale;
};
