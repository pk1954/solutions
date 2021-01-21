// win32_MainWindow.h : 
//
// NNetWindows

#pragma once

#include "Resource.h"
#include "BaseKnot.h"
#include "NNetModelReaderInterface.h"
#include "win32_NNetWindow.h"

class BeaconAnimation;
class NNetModelCommands;

class MainWindow : public NNetWindow
{
public:

	void Start
	(
		HWND                       const,
		DWORD                      const,
		bool                       const,
		NNetController           * const,
		NNetModelReaderInterface * const,
		NNetModelCommands        * const,
		Observable               * const,
		Observable               * const,
		BeaconAnimation          * const
	);

	void Stop( );
	void Reset( );

	virtual long AddContextMenuEntries( HMENU const );

	ShapeId const GetTargetShapeId     ( ) const { return m_shapeTarget; }
	ShapeId const GetHighlightedShapeId( ) const { return m_shapeHighlighted; }

	virtual bool Zoom( MicroMeter const, PixelPoint const * const );

	void ZoomStep( bool const, PixelPoint const * const );

	template <typename T>
	void NNetMove( T const & delta )	
	{ 
		GetDrawContext().Move( delta ); 
		Notify( true );  
		if ( m_pCoordObservable )
			m_pCoordObservable->NotifyAll( false );
	}

	MicroMeterPoint const GetCursorPos( ) const;

	void CenterModel( );
	void CenterSelection( );

	virtual bool OnSize              ( WPARAM const, LPARAM const );
	virtual bool OnRButtonDown       ( WPARAM const, LPARAM const );
	virtual void OnMouseWheel        ( WPARAM const, LPARAM const );
	virtual void OnMouseMove         ( WPARAM const, LPARAM const );
	virtual void OnLeftButtonDblClick( WPARAM const, LPARAM const );
	virtual void OnLButtonUp         ( WPARAM const, LPARAM const );
	virtual bool OnRButtonUp         ( WPARAM const, LPARAM const );
	virtual void OnChar              ( WPARAM const, LPARAM const );
	virtual void OnPaint             ( );

private:

	SmoothMoveFp        m_smoothMove           { };       
	MicroMeterRect      m_rectSelection        { };
	ShapeId             m_shapeTarget          { };
	ShapeId             m_shapeHighlighted     { };
	Observable        * m_pCoordObservable     { nullptr };
	Observable        * m_pCursorPosObservable { nullptr };
	NNetModelCommands * m_pNNetCommands        { nullptr };

	void centeringStep      ( );
	void setTargetShape     ( MicroMeterPoint const & );
	void setHighlightedShape( MicroMeterPoint const & );
	bool changePulseRate    ( ShapeId const, bool const );
	void centerAndZoomRect  ( ShapeList::SelMode const, float const );

	virtual void doPaint( );

	friend void CALLBACK timerProc( HWND, UINT, UINT, DWORD );

};
