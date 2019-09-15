// win32_histWindow.h
//
// Win32_history

#pragma once

#include "HistoryGeneration.h"
#include "win32_baseWindow.h"

class WorkThread;
class GenDisplayWindow;
class HistorySystem;
class HistoryIterator;
class WorkThreadInterface;
class RootWinDisplayFunctor;

class HistWindow : public BaseWindow
{
public:
    HistWindow( );
	virtual ~HistWindow( );

	void Start
	( 
		HWND const, 
		HistorySystem *, 
		WorkThreadInterface * const
	);
	void Stop( );

private:

    virtual LRESULT UserProc( UINT const, WPARAM const, LPARAM const );

    static COLORREF const CLR_GREEN  = RGB(   0, 255,   0 );
    static COLORREF const CLR_YELLOW = RGB( 255, 255,   0 );
    static COLORREF const CLR_RED    = RGB( 255,  50,  50 );
    static COLORREF const CLR_BACK   = RGB( 240, 240, 240 );
    static COLORREF const CLR_MIX    = RGB( 100, 100, 100 );
    static COLORREF const CLR_DARK   = RGB(  32,  32,  32 );
    static COLORREF const CLR_POI    = RGB(  32,  32, 255 );
    static COLORREF const CLR_EDIT   = RGB( 255, 128,   0 );

	void gotoGeneration( HIST_GENERATION const );
	void doPaint( HDC const );

	void paintAllGenerations      ( HDC const );
	void paintHighlightGenerations( HDC const, HIST_GENERATION const ) const;
	void paintLifeLine            ( HDC const, HIST_GENERATION const, HIST_GENERATION const ) const;

	PixelRect getGenerationRect( HIST_GENERATION const ) const;
    PixelRect getGenerationRect( HIST_GENERATION const, HIST_GENERATION const ) const;

    void paintGeneration( HDC const, HIST_GENERATION const, COLORREF const ) const;
    void paintPixelPos  ( HDC const, PIXEL const )                           const;

	void            gotoNewGeneration( LPARAM const );
    HIST_GENERATION getGenFromXpos( LPARAM const) const;
    void            dispGenerationWindow( )       const;

	WorkThreadInterface * m_pWorkThreadInterface;
    HistorySystem       * m_pHistSys;             // Do not change the order
    HistoryIterator     * m_pHistIter;            // declarations!
	GenDisplayWindow    * m_pGenDisplay;
    TRACKMOUSEEVENT       m_trackStruct;
    HIST_GENERATION       m_genSelected;
};
