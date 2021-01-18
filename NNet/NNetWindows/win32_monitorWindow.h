// win32_monitorWindow.h : 
//
// NNetWindows

#pragma once

#include <vector>
#include "D2D_DrawContext.h"
#include "Scale.h"
#include "MoreTypes.h"
#include "Measurement.h"
#include "MonitorData.h"
#include "win32_baseWindow.h"
#include "win32_NNetController.h"

class BeaconAnimation;
class NNetModelReaderInterface;

class MonitorWindow : public BaseWindow
{
public:

	void Start
	( 
		HWND                     const, 
		Sound                  * const,
		NNetController         * const,
		NNetModelReaderInterface const &, 
		MonitorData                    &
	);

	void Reset( );
	void Stop( );

	virtual long AddContextMenuEntries( HMENU const );

	void AddSignal( MicroMeterCircle const &  );
	void InsertTrack( TrackNr const );

private:

	SignalNr const findSignal      ( TrackNr const, PixelPoint const & ) const;
	fPixel   const calcTrackHeight ( ) const;
	TrackNr  const findTrack       ( PIXEL const ) const;
	TrackNr  const findPos4NewTrack( PIXEL const ) const;

	fMicroSecs const fPixel2fMicroSecs( fPixel     const ) const;
	fPixel     const fMicroSecs2fPixel( fMicroSecs const ) const;

	fPixel const yValue2fPixel( float const fYvalue ) const
	{
		fPixel const fPixYvalue { fYvalue / m_fYvaluesPerPixel };
		assert( fPixYvalue.GetValue() < 10000.0f );
		return fPixYvalue;
	}

	fPixel const getYvalue( Signal const & signal, fMicroSecs const time ) const
	{
		return yValue2fPixel( signal.GetDataPoint( time ) );
	}

	fMicroSecs findNextMax( Signal const & signal, fPixel const fPixX ) const
	{
		fMicroSecs const usParam { fPixel2fMicroSecs( fPixX ) };
		fMicroSecs const usMax   { signal.FindNextMaximum( usParam ) };
		return usMax;
	}

	fPixel getSignalOffset( SignalId const & idSignal ) const
	{ 
		fPixel fPixOffset { calcTrackHeight() * Cast2Float(idSignal.GetTrackNr().GetValue()+1) }; 
		if ( idSignal == m_idSigSelected )  
			fPixOffset += Convert2fPixel( m_pixMoveOffsetY );  // may have move offset
		return fPixOffset;
	};

	void doPaint( ) const;
	void selectSignal( SignalId const & );
	void deselectSignal( );
	void paintSignal( SignalId const & ) const;
	void drawDiamond( SignalId const & ) const;

	virtual void OnPaint( );
	virtual bool OnSize              ( WPARAM const, LPARAM const );
	virtual void OnMouseWheel        ( WPARAM const, LPARAM const );
	virtual void OnMouseMove         ( WPARAM const, LPARAM const );
	virtual bool OnMouseLeave        ( WPARAM const, LPARAM const );
	virtual void OnLButtonUp         ( WPARAM const, LPARAM const );
	virtual void OnLeftButtonDblClick( WPARAM const, LPARAM const );
	virtual bool OnShow              ( WPARAM const, LPARAM const );
	virtual bool OnCommand           ( WPARAM const, LPARAM const, PixelPoint const = PixelPoint::NULL_VAL() );
	virtual void OnChar              ( WPARAM const, LPARAM const ) { };

	inline static HCURSOR m_hCrsrWE { nullptr };
	inline static HCURSOR m_hCrsrNS { nullptr };

	Sound                          * m_pSound       { nullptr };
	NNetController                 * m_pController  { nullptr };
	NNetModelReaderInterface const * m_pNMRI        { nullptr };
	MonitorData                    * m_pMonitorData { nullptr };  

	TRACKMOUSEEVENT m_trackStruct { sizeof(TRACKMOUSEEVENT), TME_LEAVE, HWND(0), 0L };

	D2D_driver  m_graphics           { };
	fMicroSecs  m_fMicroSecsPerPixel { 100.0_MicroSecs };
	float       m_fYvaluesPerPixel   { 0.2f };
	SignalId    m_idSigSelected      { };
	TrackNr     m_trackNrHighlighted { TrackNr::NULL_VAL() };
	PixelPoint  m_pixLast            { PP_NULL };     // last cursor position during selection 
	PIXEL       m_pixMoveOffsetY     { 0_PIXEL };     // vertical offset when moving signal
	fPixel      m_fPixWinWidth       { 0.0_fPixel };
	bool        m_bShowScale         { false };
	bool        m_bSignalLocked      { false };
	Measurement m_measurement;
	Scale       m_scale;
};
