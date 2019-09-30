// win32_NNetWindow.h : 
//
// NNetWindows

#pragma once

#include <sstream> 
#include "NNetTypes.h"
#include "NNetReadBuffer.h"  // do not remove! class definition not sufficient.
#include "NNetPixelCoords.h"
#include "win32_modelWindow.h"

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
	static COLORREF const SCALE_COLOR = RGB( 0, 0, 0 );  // CLR_BLACK

	NNetWindow             ( NNetWindow const & );  // noncopyable class 
	NNetWindow & operator= ( NNetWindow const & );  // noncopyable class 

	static NNetReadBuffer          * m_pReadBuffer;
	static NNetWorkThreadInterface * m_pNNetWorkThreadInterface;

	std::wostringstream m_wBuffer;

	HMENU m_hPopupMenu;
	BOOL  m_bMoveAllowed;    // TRUE: move with mouse is possible

	GraphicsInterface * m_pGraphics;

	NNetPixelCoords m_NNetPixelCoords;
	PixelPoint      m_ptLast;	 	   // Last cursor position during selection 

	void newPixelSize( NanoMeter const, NNetPoint const );
	void moveNNet( PixelPoint const );
	void showScale( HDC );
	void displayTicks( fPixelPoint const, fPixelPoint const, double const, int const );
	void displayScaleNumber( fPixelPoint const, double const, int const );
	void displayScaleText( fPixelPoint const, double const );

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
