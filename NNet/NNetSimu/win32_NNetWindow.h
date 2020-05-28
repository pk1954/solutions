// win32_NNetWindow.h : 
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "ShapeId.h"
#include "DrawContext.h"
#include "tHighlightType.h"
#include "SmoothMoveFp.h"
#include "win32_NNetController.h"
#include "win32_modelWindow.h"

using std::wstring;
using std::function;

class Scale;
class DrawModel;
class Observable;
class ActionTimer;
class ComputeThread;
class ObserverInterface;
class ControllerInterface;
class NNetModelReaderInterface;
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
		ComputeThread            * const,
		NNetController           * const,
		NNetModelReaderInterface * const,
		NNetModelWriterInterface * const,
		DrawModel                * const,
		Observable               * const
	);

	void Stop( );

	virtual ~NNetWindow( );

	virtual void NNetMove( PixelPoint const & );
	virtual bool Zoom( MicroMeter const );

	ShapeId        const GetHighlightedShapeId( )          const { return m_shapeHighlighted; }
	ShapeId        const GetSuperHighlightedShapeId( )     const { return m_shapeSuperHighlighted; }
	tHighlightType const GetHighlightType( Shape const & ) const;

	void ResetHighlightedShape( ) { m_shapeHighlighted = NO_SHAPE; }
	void ZoomStep( bool const );
	void CenterModel( bool const );
	void CenterAndZoomRect( MicroMeterRect const &, float const, bool const );

	DrawContext       & GetDrawContext ()       { return m_context; }
	DrawContext const & GetDrawContextC() const { return m_context; }

protected:

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

	DrawContext   m_context    { };
	DrawModel   * m_pDrawModel { nullptr };
	PixelPoint    m_ptLast     { PP_NULL };	// Last cursor position during selection 

	NNetModelReaderInterface * m_pModelReaderInterface { nullptr };

private:

	NNetWindow             ( NNetWindow const & );  // noncopyable class 
	NNetWindow & operator= ( NNetWindow const & );  // noncopyable class 

	NNetModelWriterInterface * m_pModelWriterInterface { nullptr };

	ComputeThread   * m_pComputeThread       { nullptr };
	NNetController  * m_pController          { nullptr };
	Observable      * m_pCursorPosObservable { nullptr };
	HMENU             m_hPopupMenu           { nullptr };

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
