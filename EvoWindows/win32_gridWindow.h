// win32_gridWindow.h : 
//

#pragma once

#include "win32_baseWindow.h"

class GraphicsInterface;
class DrawFrame;
class PixelCoordinates;
class WorkThreadInterface;
class DspOptWindow;
class EvolutionCore;
class ObserverInterface;
class PerformanceWindow;
class FocusPoint;
class ColorManager;

class GridWindow : public BaseWindow
{
public:
	static void InitClass
	( 
		HWND                  const, 
		EvolutionCore       * const,
        WorkThreadInterface * const,
        FocusPoint          * const,
        DspOptWindow        * const,
        PerformanceWindow   * const, 
		ColorManager        * const 
	);

    GridWindow( );
    ~GridWindow( );

    void Start( GraphicsInterface * const, DWORD const, PIXEL const );

    void  Size( );
	void  Zoom( bool const );
	void  ToggleStripMode( );
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

private:
    GridWindow             ( GridWindow const & );  // noncopyable class 
    GridWindow & operator= ( GridWindow const & );  // noncopyable class 

	static HWND                  m_hwndApp;
	static EvolutionCore       * m_pCore;
    static WorkThreadInterface * m_pWorkThreadInterface;
    static PerformanceWindow   * m_pPerformanceWindow;
    static DspOptWindow        * m_pDspOptWindow;
    static FocusPoint          * m_pFocusPoint;
	static ColorManager        * m_pColorManager;

	GraphicsInterface * m_pGraphics;
    PixelCoordinates  * m_pPixelCoordinates;   // My own PixelCoordinates
    GridWindow        * m_pGridWindowObserved; // Observed GridWindow (or nullptr)
    ObserverInterface * m_pObserverInterface;
    DrawFrame         * m_pDrawFrame;
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
