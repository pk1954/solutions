// win32_MainWindow.h : 
//
// NNetWindows

#pragma once

#include "Resource.h"
#include "BaseKnot.h"
#include "NNetModelReaderInterface.h"
#include "win32_NNetWindow.h"

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
		Observable               * const
	);

	void Stop( );
	void Reset( );

	virtual long AddContextMenuEntries( HMENU const );

	ShapeId const GetSuperHighlightedShapeId( ) const { return m_shapeSuperHighlighted; }
	ShapeId const GetHighlightedShapeId( )      const { return m_shapeHighlighted; }

	virtual bool Zoom( MicroMeter const, PixelPoint const * const );

	void ZoomStep( bool const, PixelPoint const * const );

	void NNetMove( MicroMeterPoint const & );
	void NNetMove( PixelPoint      const & ); 

	MicroMeterPoint const GetCursorPos( ) const;

	void CenterModel( );

	virtual void CenterAndZoomRect( MicroMeterRect const &, float const );

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

	SmoothMoveFp        m_smoothMove            { };       
	MicroMeterRect      m_rectSelection         { };
	ShapeId             m_shapeSuperHighlighted { NO_SHAPE };
	ShapeId             m_shapeHighlighted      { NO_SHAPE };
	Observable        * m_pCoordObservable      { nullptr };
	Observable        * m_pCursorPosObservable  { nullptr };
	NNetModelCommands * m_pNNetCommands         { nullptr };
	bool                m_bFocusMode            { false };

	void setSuperHighlightedShape( MicroMeterPoint const & );
	void setHighlightedShape     ( MicroMeterPoint const & );
	bool changePulseRate         ( ShapeId const, bool const );

	virtual void doPaint( );
};
