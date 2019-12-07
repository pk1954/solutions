// win32_NNetWindow.h : 
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "Shape.h"
#include "PixelCoordsFp.h"
#include "NNetReadBuffer.h"  // do not remove! class definition not sufficient.
#include "win32_modelWindow.h"

using std::wstring;

class Scale;
class ActionTimer;
class GraphicsInterface;
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
		HWND                  const, 
		GraphicsInterface   * const, 
		DWORD                 const,
		std::function<bool()> const
	);

	void Stop( );

	~NNetWindow( );

//	void      Size( );
	void       Zoom( bool const );
	void       SetPixelSize( MicroMeter const );
	MicroMeter GetPixelSize( ) const;

	MicroMeterPoint PixelPoint2MicroMeterPoint( PixelPoint const ) const;

	float ParameterDialog( tParameter param );

private:
	NNetWindow             ( NNetWindow const & );  // noncopyable class 
	NNetWindow & operator= ( NNetWindow const & );  // noncopyable class 

	static PixelCoordsFp             m_coord;
	static NNetReadBuffer          * m_pReadBuffer;
	static NNetWorkThreadInterface * m_pNNetWorkThreadInterface;

	HMENU m_hPopupMenu;
	BOOL  m_bMoveAllowed;    // TRUE: move with mouse is possible

	GraphicsInterface * m_pGraphics;
	Scale             * m_pScale;

	PixelPoint m_ptLast;	 	   // Last cursor position during selection 

	void moveNNet( PixelPoint const );

	virtual void AddContextMenuEntries( HMENU const, PixelPoint const );

	virtual void OnLeftButtonDblClick( WPARAM const, LPARAM const );
	virtual void OnMouseWheel        ( WPARAM const, LPARAM const );
	virtual void OnMouseMove         ( WPARAM const, LPARAM const );
	virtual BOOL OnCommand           ( WPARAM const, LPARAM const );
	virtual void OnLButtonDown       ( WPARAM const, LPARAM const );
	virtual void OnLButtonUp         ( WPARAM const, LPARAM const );
	virtual void OnSetCursor         ( WPARAM const, LPARAM const );
	virtual void OnPaint( );

    LPARAM        crsPos2LPARAM( ) const;
	LPARAM        pixelPoint2LPARAM( PixelPoint const ) const;
	BOOL          inObservedClientRect( LPARAM const );
	ShapeId const getShapeUnderPoint( PixelPoint const );
	void          drawHighlightedShape( NNetModel const & , PixelCoordsFp & );
	void          doPaint( );
};
