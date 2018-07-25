// win32_histWindow.h : 
//

#pragma once

#include "win32_baseWindow.h"

class EvolutionCore;
class WorkThread;
class GenDisplayWindow;
class HistorySystem;
class HistoryIterator;
class RootWinDisplayFunctor;

class HistWindow : public BaseWindow
{
public:
    HistWindow( );
	virtual ~HistWindow( );

protected:
    void Start( HWND const, HistorySystem * );

    void PaintAllGenerations      ( HDC const );
    void PaintHighlightGenerations( HDC const, HIST_GENERATION const )                                        const;
	void PaintLifeLine            ( HDC const, HIST_GENERATION const, HIST_GENERATION const ) const;

	// callbacks

	virtual void DoPaint       ( HDC const )             = 0;
    virtual void GotoGeneration( HIST_GENERATION const ) = 0;

private:

    virtual LRESULT UserProc( UINT const, WPARAM const, LPARAM const );

    static COLORREF const CLR_GREEN  = RGB( 0, 255, 0 );
    static COLORREF const CLR_YELLOW = RGB( 255, 255, 0 );
    static COLORREF const CLR_RED    = RGB( 255, 50, 50 );
    static COLORREF const CLR_BACK   = RGB( 240, 240, 240 );
    static COLORREF const CLR_MIX    = RGB( 100, 100, 100 );
    static COLORREF const CLR_DARK   = RGB( 32, 32, 32 );
    static COLORREF const CLR_POI    = RGB( 32, 32, 255 );
    static COLORREF const CLR_EDIT   = RGB( 255, 128, 0 );

    RECT getGenerationRect( HIST_GENERATION const ) const;
    RECT getGenerationRect( HIST_GENERATION const, HIST_GENERATION const ) const;

    void paintGeneration( HDC const, HIST_GENERATION const, COLORREF const ) const;
    void paintPixelPos  ( HDC const, long const )                            const;

	void            gotoNewGeneration( LPARAM const );
    HIST_GENERATION getGenFromXpos( LPARAM const) const;
    void            dispGenerationWindow( )       const;

    HistorySystem    * m_pHistSys;  // Do not change the order
    HistoryIterator  * m_pHistIter;  // declarations!
    GenDisplayWindow * m_pGenDisplay;
    TRACKMOUSEEVENT    m_trackStruct;
    HIST_GENERATION    m_genSelected;
};
