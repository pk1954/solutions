// win32_gridWindow.h : 
//

#pragma once

#include "GridPoint.h"
#include "pixelCoordinates.h"
#include "EvoReadBuffer.h"
#include "win32_baseWindow.h"
#include "win32_draw.h"

class EvolutionCore;
class GraphicsInterface;
class EvoWorkThreadInterface;
class DspOptWindow;
class ObserverInterface;
class ActionTimer;
class FocusPoint;
class ColorManager;

class GridWindow : public BaseWindow
{
public:
	static void InitClass
	( 
		EvoReadBuffer          * const,
        EvoWorkThreadInterface * const,
        FocusPoint             * const,
        DspOptWindow           * const,
        ActionTimer            * const, 
		ColorManager           * const 
	);

    GridWindow( );

    void Start
	( 
		HWND                  const, 
		GraphicsInterface   * const, 
		DWORD                 const, 
		PIXEL                 const, 
		std::function<bool()> const
	);
	void Stop( );

	~GridWindow( );
	
	void  Size( );
	void  Zoom( bool const );
	void  ToggleStripMode( );
	void  ToggleClutMode( );
	void  Escape( );
	void  SetFieldSize( PIXEL const );
	void  Fit2Rect( );
	bool  IsFullGridVisible( ) const;
	PIXEL GetFieldSize( ) const;

	void Observe( GridWindow * const pGridWin )
	{
		m_pGridWindowObserved = pGridWin;
		m_bMoveAllowed = FALSE; 
	}

	void Refresh( )
	{
		Trigger( );
	}

private:
    GridWindow             ( GridWindow const & );  // noncopyable class 
    GridWindow & operator= ( GridWindow const & );  // noncopyable class 

	static HWND                     m_hwndApp;
	static HCURSOR                  m_hCrsrMove;
	static HCURSOR                  m_hCrsrArrow;
	static EvoReadBuffer          * m_pReadBuffer;
    static EvoWorkThreadInterface * m_pEvoWorkThreadInterface;
    static ActionTimer            * m_pActionTimer;
    static DspOptWindow           * m_pDspOptWindow;
    static FocusPoint             * m_pFocusPoint;
	static ColorManager           * m_pColorManager;

	PixelCoordinates m_PixelCoordinates;   // My own PixelCoordinates
	DrawFrame        m_DrawFrame;

	GraphicsInterface * m_pGraphics;
    GridWindow        * m_pGridWindowObserved; // Observed GridWindow (or nullptr)
    ObserverInterface * m_pObserverInterface;
    PixelPoint 	        m_ptLast;	 	   // Last cursor position during selection 
    BOOL                m_bMoveAllowed;    // TRUE: move with mouse is possible
    HMENU               m_hPopupMenu;

    virtual LRESULT UserProc( UINT const, WPARAM const, LPARAM const );
	virtual void    AddContextMenuEntries( HMENU const, POINT const );

	void newFieldSize( PIXEL const, GridPoint const );
	void mouseWheelAction( WPARAM const  );
    BOOL inObservedClientRect( LPARAM const );
    void moveGrid( PixelPoint const );
    void onMouseMove( LPARAM const, WPARAM const );
    void doPaint( );
};
