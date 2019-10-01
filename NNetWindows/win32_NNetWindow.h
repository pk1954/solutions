// win32_NNetWindow.h : 
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "fPixelCoords.h"
#include "NNetReadBuffer.h"  // do not remove! class definition not sufficient.
#include "win32_modelWindow.h"

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
		NanoMeter             const,
		std::function<bool()> const
	);

	void Stop( );

	~NNetWindow( );

//	void      Size( );
	void      Zoom( bool const );
	void      SetPixelSize( NanoMeter const );
	NanoMeter GetPixelSize( ) const;

private:
	NNetWindow             ( NNetWindow const & );  // noncopyable class 
	NNetWindow & operator= ( NNetWindow const & );  // noncopyable class 

	static NNetReadBuffer          * m_pReadBuffer;
	static NNetWorkThreadInterface * m_pNNetWorkThreadInterface;

	HMENU m_hPopupMenu;
	BOOL  m_bMoveAllowed;    // TRUE: move with mouse is possible

	GraphicsInterface * m_pGraphics;
	Scale             * m_pScale;

	fPixelCoords m_fPixelCoords;
	PixelPoint   m_ptLast;	 	   // Last cursor position during selection 

	void newPixelSize( NanoMeter const, MicroMeterPoint const );
	void moveNNet( PixelPoint const );

	virtual void AddContextMenuEntries( HMENU const, POINT const );

	virtual void OnMouseWheel ( WPARAM const, LPARAM const );
	virtual void OnMouseMove  ( WPARAM const, LPARAM const );
	virtual BOOL OnCommand    ( WPARAM const, LPARAM const );
	virtual void OnLButtonDown( WPARAM const, LPARAM const );
	virtual void OnLButtonUp  ( WPARAM const, LPARAM const );
	virtual void OnSetCursor  ( WPARAM const, LPARAM const );
	virtual void OnPaint( );

	BOOL inObservedClientRect( LPARAM const );
};
