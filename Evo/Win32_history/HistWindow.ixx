// HistWindow.ixx
//
// Win32_history

export module WinHistory:HistWindow;

import BaseWindow;
import HistoryLib;
import WorkThreadInterface;
import RootWinDisplayFunctor;

class HistWindow : public BaseWindow
{
public:
    HistWindow();
	virtual ~HistWindow();

	void Start
	(
		HWND const, 
		HistorySystem *, 
		WorkThreadInterface * const
	);
	void Stop();

private:

    virtual bool UserProc(UINT const, WPARAM const, LPARAM const);

    //static COLORREF const CLR_GREEN  = RGB(  0, 255,   0);
    //static COLORREF const CLR_YELLOW = RGB(255, 255,   0);
    //static COLORREF const CLR_RED    = RGB(255,  50,  50);
    //static COLORREF const CLR_BACK   = RGB(240, 240, 240);
    //static COLORREF const CLR_MIX    = RGB(100, 100, 100);
    //static COLORREF const CLR_DARK   = RGB( 32,  32,  32);
    //static COLORREF const CLR_POI    = RGB( 32,  32, 255);
    //static COLORREF const CLR_EDIT   = RGB(255, 128,   0);

	void gotoGeneration(HistGeneration const);
	void doPaint(HDC const);

	void paintAllGenerations      (HDC const);
	void paintHighlightGenerations(HDC const, HistGeneration const) const;
	void paintLifeLine            (HDC const, HistGeneration const, HistGeneration const) const;

	PixelRect getGenerationRect(HistGeneration const) const;
    PixelRect getGenerationRect(HistGeneration const, HistGeneration const) const;

    void paintGeneration(HDC const, HistGeneration const, COLORREF const) const;
    void paintPixelPos  (HDC const, PIXEL const, HistoryIterator & )       const;

	void            gotoNewGeneration(LPARAM const);
    HistGeneration getGenFromXpos(LPARAM const) const;
    void            dispGenerationWindow()       const;

	WorkThreadInterface * m_pWorkThreadInterface;
    HistorySystem       * m_pHistSys;       
	GenDisplayWindow    * m_pGenDisplay;
    TRACKMOUSEEVENT       m_trackStruct;
    HistGeneration        m_genSelected;
};
