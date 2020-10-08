// win32_monitorWindow.h : 
//
// NNetWindows

#pragma once

#include <vector>
#include "D2D_DrawContext.h"
#include "MonitorData.h"
#include "win32_baseWindow.h"

class Param;
class BeaconAnimation;
class NNetModelReaderInterface;

class MonitorWindow : public BaseWindow
{
public:
	void Start
	( 
		HWND const, 
		NNetModelReaderInterface const &, 
		Param                    const &,
		BeaconAnimation                &,
		MonitorData                    &
	);

	void Reset( );
	void Stop( );

	virtual long AddContextMenuEntries( HMENU const );

private:

	virtual void OnPaint( );
	virtual bool OnSize       ( WPARAM const, LPARAM const );
	virtual void OnMouseWheel ( WPARAM const, LPARAM const );
	virtual bool OnRButtonUp  ( WPARAM const, LPARAM const ) { return false; }
	virtual bool OnRButtonDown( WPARAM const, LPARAM const ) { return false; }
	virtual void OnLButtonDown( WPARAM const, LPARAM const ) { };
	virtual void OnChar       ( WPARAM const, LPARAM const ) { };
	virtual void OnMouseMove  ( WPARAM const, LPARAM const );
	virtual bool OnMouseLeave ( WPARAM const, LPARAM const );
	virtual void OnLButtonUp  ( WPARAM const, LPARAM const );
	virtual bool OnCommand    ( WPARAM const, LPARAM const, PixelPoint const = PixelPoint::NULL_VAL() );

	void doPaint( );
	void moveSignal( PIXEL const );
	void addTrack   ( TrackNr const );
	void selectSignal( Signal * const );
	void paintSignal ( Signal const &, fPIXEL const, fPIXEL const, fMicroSecs const, fMicroSecs const );

	fPIXEL  const calcTrackHeight ( );
	fPIXEL  const getYvalue       ( Signal const &, fMicroSecs const );
	TrackNr const findTrack       ( PIXEL const );
	TrackNr const findPos4NewTrack( PIXEL const );
	Signal      * findSignal      ( TrackNr const );
	bool          testSignal      ( Signal const &, fMicroSecs const, fPIXEL const, fPIXEL & );

	TRACKMOUSEEVENT                  m_trackStruct        { sizeof(TRACKMOUSEEVENT), 0, HWND(0), 0L };
	bool                             m_bRuler             { true };
	D2D_driver                       m_graphics           { };
	Param                    const * m_pParams            { nullptr };
	NNetModelReaderInterface const * m_pModel             { nullptr };
	BeaconAnimation                * m_pBeaconAnimation   { nullptr };
	MonitorData                    * m_pMonitorData       { nullptr };  
	fMicroSecs                       m_fMicroSecsPerPixel { 100.0_MicroSecs };
	float                            m_fYvaluesPerPixel   { 0.2f };
	Signal                         * m_pSelectedSignal    { nullptr };
	TrackNr                          m_selectedTrackNr    { TrackNr::NULL_VAL() };
	TrackNr                          m_trackNrOfSelSignal { TrackNr::NULL_VAL() };
	PIXEL                            m_pixLastY           { PIXEL::NULL_VAL() };	// Last cursor position during selection 
	PIXEL                            m_pixMoveOffsetY     { 0_PIXEL };
};
