// HistWindow.ixx
//
// Win32_history

export module WinHistLib:HistWindow;

import WinBasics;
import BaseWindow;
import HistoryLib;
import :GenDisplayWindow;
//import WorkThread;

export class HistWindow : public BaseWindow
{
public:
    HistWindow();
	virtual ~HistWindow();

	void Start
	(
		HWND const, 
		HistorySystem *  //, 
		//WorkThread * const
	);
	void Stop();

private:

    virtual bool UserProc(UINT const, WPARAM const, LPARAM const);

    static COLORREF const CLR_GREEN  = MakeRGB(  0, 255,   0);
    static COLORREF const CLR_YELLOW = MakeRGB(255, 255,   0);
    static COLORREF const CLR_RED    = MakeRGB(255,  50,  50);
    static COLORREF const CLR_BACK   = MakeRGB(240, 240, 240);
    static COLORREF const CLR_MIX    = MakeRGB(100, 100, 100);
    static COLORREF const CLR_DARK   = MakeRGB( 32,  32,  32);
    static COLORREF const CLR_POI    = MakeRGB( 32,  32, 255);
    static COLORREF const CLR_EDIT   = MakeRGB(255, 128,   0);

	void gotoGeneration(HistGeneration const);
	void doPaint(HDC const);

	void paintAllGenerations      (HDC const);
	void paintHighlightGenerations(HDC const, HistGeneration const) const;
	void paintLifeLine            (HDC const, HistGeneration const, HistGeneration const) const;

	PixelRect getGenerationRect(HistGeneration const) const;
    PixelRect getGenerationRect(HistGeneration const, HistGeneration const) const;

    void paintGeneration(HDC const, HistGeneration const, COLORREF const) const;
    void paintPixelPos  (HDC const, PIXEL const, HistoryIterator &)       const;

	void           gotoNewGeneration(LPARAM const);
    HistGeneration getGenFromXpos(LPARAM const) const;
    void           dispGenerationWindow()       const;

	//WorkThread * m_pWorkThread;
    HistorySystem       * m_pHistSys;       
	GenDisplayWindow    * m_pGenDisplay;
    TRACKMOUSEEVENT       m_trackStruct;
    HistGeneration        m_genSelected;
};
