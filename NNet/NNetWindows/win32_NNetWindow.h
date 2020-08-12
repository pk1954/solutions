// win32_NNetWindow.h : 
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "ShapeId.h"
#include "D2D_DrawContext.h"
#include "tHighlightType.h"
#include "SmoothMoveFp.h"
#include "win32_NNetController.h"
#include "win32_modelWindow.h"
#include "NNetModelReaderInterface.h"

using std::wstring;
using std::function;

class Scale;
class DrawModel;
class Observable;
class ActionTimer;
class ObserverInterface;
class ControllerInterface;
class NNetModelWriterInterface;

class NNetWindow : public ModelWindow
{
public:
	static void InitClass( ActionTimer * const );

	NNetWindow( );

	void Start
	( 
		HWND                       const, 
		DWORD                      const,
		bool                       const,
		NNetController           * const,
		NNetModelReaderInterface * const,
		NNetModelWriterInterface * const,
		DrawModel                * const,
		Observable               * const
	);

	void Stop( );

	virtual ~NNetWindow( );

	virtual bool Zoom( MicroMeter const, PixelPoint const * const );
	virtual void NNetMove( PixelPoint const & );

	MicroMeterRect const GetViewRect() const;
	tHighlightType const GetHighlightType( Shape const & ) const;

	void Reset( );
	void ZoomStep( bool const, PixelPoint const * const );
	void CenterModel( bool const );
	void CenterAndZoomRect( MicroMeterRect const &, float const, bool const );

	DrawContext const & GetDrawContextC() const { return m_context; }
	DrawContext       & GetDrawContext ()       { return m_context; }

	ShapeId const GetHighlightedShapeId( )      const { return m_shapeHighlighted; }
	ShapeId const GetSuperHighlightedShapeId( ) const { return m_shapeSuperHighlighted; }

protected:

	DrawModel      const * GetDrawModel    () const { return m_pDrawModel; }
	MicroMeterRect const   GetEnclosingRect() const { return m_pModelReaderInterface->GetEnclosingRect(); }

	virtual long AddContextMenuEntries( HMENU const );

	virtual bool OnCommand           ( WPARAM const, LPARAM const, PixelPoint const );
	virtual void OnLeftButtonDblClick( WPARAM const, LPARAM const );
	virtual void OnMouseWheel        ( WPARAM const, LPARAM const );
	virtual void OnMouseMove         ( WPARAM const, LPARAM const );
	virtual void OnLButtonUp         ( WPARAM const, LPARAM const );
	virtual bool OnRButtonUp         ( WPARAM const, LPARAM const );
	virtual void OnSetCursor         ( WPARAM const, LPARAM const );
	virtual void OnSize              ( WPARAM const, LPARAM const );
	virtual void OnLButtonDown       ( WPARAM const, LPARAM const ) {}
	virtual bool OnRButtonDown       ( WPARAM const, LPARAM const );
	virtual void OnPaint( );

	virtual void doPaint( );
	virtual void smoothStep( );

	PixelPoint  m_ptLast  { PP_NULL };	// Last cursor position during selection 

	D2D_DrawContext m_context { };
private:

	NNetWindow             ( NNetWindow const & );  // noncopyable class 
	NNetWindow & operator= ( NNetWindow const & );  // noncopyable class 

	DrawModel                * m_pDrawModel            { nullptr };
	NNetModelWriterInterface * m_pModelWriterInterface { nullptr };
	NNetModelReaderInterface * m_pModelReaderInterface { nullptr };
	NNetController           * m_pController           { nullptr };
	Observable               * m_pCursorPosObservable  { nullptr };
	HMENU                      m_hPopupMenu            { 0 };

	bool m_bFocusMode { false };

	MicroMeterPoint m_umPntCenterStart { MicroMeterPoint::NULL_VAL() }; // SmoothMove TODO: move these variables to SmootMoveFp
	MicroMeterPoint m_umPntCenterDelta { MicroMeterPoint::NULL_VAL() }; // SmoothMove 
	MicroMeter      m_umPixelSizeStart { MicroMeter::NULL_VAL() };      // SmoothMove 
	MicroMeter      m_umPixelSizeDelta { MicroMeter::NULL_VAL() };      // SmoothMove 
	SmoothMoveFp    m_smoothMove { };                                   // SmoothMove   

	MicroMeterRect m_rectSelection { };

	ShapeId m_shapeHighlighted      { NO_SHAPE };
	ShapeId m_shapeSuperHighlighted { NO_SHAPE };

	void setSuperHighlightedShape( MicroMeterPoint const & );
	void setHighlightedShape     ( MicroMeterPoint const & );
};
