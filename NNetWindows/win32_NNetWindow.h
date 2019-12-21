// win32_NNetWindow.h : 
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "Shape.h"
#include "Direct2D.h"
#include "PixelCoordsFp.h"
#include "NNetReadBuffer.h"  // do not remove! class definition not sufficient.
#include "tHighlightType.h"
#include "win32_modelWindow.h"

using std::wstring;
using std::function;

class Scale;
class ActionTimer;
class NNetWorkThreadInterface;

class NNetWindow : public ModelWindow
{
public:
	static void InitClass
	( 
		NNetReadBuffer          * const,
		NNetWorkThreadInterface * const,
		ActionTimer             * const
	);

	NNetWindow( );

	void Start
	( 
		HWND             const, 
		DWORD            const,
		function<bool()> const
	);

	void Stop( );

	~NNetWindow( );

	ShapeId        const GetHighlightedShapeId( )          const { return m_shapeHighlighted; }
	ShapeId        const GetSuperHighlightedShapeId( )     const { return m_shapeSuperHighlighted; }
	tHighlightType const GetHighlightType( ShapeId const ) const;

	void       ResetHighlightedShape( ) { m_shapeHighlighted = NO_SHAPE; }
	void       Zoom( bool const );
	void       SetPixelSize( MicroMeter const );
	MicroMeter GetPixelSize( ) const;

	MicroMeterPoint PixelPoint2MicroMeterPoint( PixelPoint const ) const;

	void ParameterDlg( tParameter param );

private:
	NNetWindow             ( NNetWindow const & );  // noncopyable class 
	NNetWindow & operator= ( NNetWindow const & );  // noncopyable class 

	static NNetReadBuffer          * m_pReadBuffer;
	static NNetWorkThreadInterface * m_pNNetWorkThreadInterface;

	HMENU m_hPopupMenu;
	BOOL  m_bMoveAllowed;    // TRUE: move with mouse is possible

	D2D_driver    m_D2d_driver;
	PixelCoordsFp m_coord;
	Scale       * m_pScale;

	PixelPoint m_ptLast;	 	   // Last cursor position during selection 
	PixelPoint m_ptCommandPosition;

	ShapeId m_shapeHighlighted;
	ShapeId m_shapeSuperHighlighted;

	virtual void AddContextMenuEntries( HMENU const, PixelPoint const );

	virtual void OnLeftButtonDblClick( WPARAM const, LPARAM const );
	virtual void OnMouseWheel        ( WPARAM const, LPARAM const );
	virtual void OnMouseMove         ( WPARAM const, LPARAM const );
	virtual BOOL OnCommand           ( WPARAM const, LPARAM const );
	virtual void OnLButtonDown       ( WPARAM const, LPARAM const );
	virtual void OnLButtonUp         ( WPARAM const, LPARAM const );
	virtual void OnSetCursor         ( WPARAM const, LPARAM const );
	virtual void OnSize              ( WPARAM const, LPARAM const );
	virtual void OnPaint( );

	void          setStdFontSize( );
	LPARAM        crsPos2LPARAM( ) const;
	LPARAM        pixelPoint2LPARAM( PixelPoint const ) const;
	BOOL          inObservedClientRect( LPARAM const );
	ShapeId const getShapeUnderPoint( PixelPoint const );
	void          drawHighlightedShape( NNetModel const &, PixelCoordsFp & );
	void          doPaint( );
};
