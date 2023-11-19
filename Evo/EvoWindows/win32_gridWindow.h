// win32_gridWindow.h : 
//
// EvoWindows

module;

#include "GridPoint.h"
#include "EvoPixelCoords.h"
#include "EvoReadBuffer.h"
#include "win32_modelWindow.h"
#include "win32_draw.h"

class EvolutionCore;
class GraphicsInterface;
class EvoWorkThreadInterface;
class DspOptWindow;
class ObserverInterface;
class ActionTimer;
class FocusPoint;
class ColorManager;

class GridWindow : public ModelWindow
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

    GridWindow();

    void Start
	(
		HWND                const, 
		GraphicsInterface * const, 
		DWORD               const, 
		PIXEL               const, 
		function<bool()>    const
	);
	void Stop();

	~GridWindow();
	
	void  Size();
	void  Zoom(bool const);
	void  ToggleStripMode();
	void  ToggleClutMode();
	void  Escape();
	void  SetFieldSize(PIXEL const);
	void  Fit2Rect();
	bool  IsFullGridVisible() const;
	PIXEL GetFieldSize() const;

	void Observe(GridWindow * const pGridWin)
	{
		m_pGridWindowObserved = pGridWin;
		m_bMoveAllowed = FALSE; 
	}

private:
    GridWindow             (GridWindow const &);  // noncopyable class 
    GridWindow & operator= (GridWindow const &);  // noncopyable class 

	static EvoReadBuffer          * m_pReadBuffer;
    static EvoWorkThreadInterface * m_pEvoWorkThreadInterface;
    static DspOptWindow           * m_pDspOptWindow;
    static FocusPoint             * m_pFocusPoint;
	static ColorManager           * m_pColorManager;

	EvoPixelCoords m_EvoPixelCoords; 
	DrawFrame      m_DrawFrame;

	GraphicsInterface * m_pGraphics;
    GridWindow        * m_pGridWindowObserved; // Observed GridWindow (or nullptr)
    ObserverInterface * m_pObserverInterface;
    PixelPoint 	        m_ptLast;	 	   // Last cursor position during selection 
    bool                m_bMoveAllowed;    // TRUE: move with mouse is possible
    HMENU               m_hPopupMenu;

	virtual long AddContextMenuEntries(HMENU const, PixelPoint const);

	virtual void OnLeftButtonDblClick(WPARAM const, LPARAM const) {};
	virtual bool OnRButtonDown(WPARAM const, LPARAM const) { return false; };
	virtual bool OnRButtonUp  (WPARAM const, LPARAM const) { return false; };
	virtual void OnMouseWheel (WPARAM const, LPARAM const);
	virtual void OnMouseMove  (WPARAM const, LPARAM const);
	virtual bool OnCommand    (WPARAM const, LPARAM const);
	virtual void OnLButtonDown(WPARAM const, LPARAM const);
	virtual void OnLButtonUp  (WPARAM const, LPARAM const);
	virtual void OnSetCursor  (WPARAM const, LPARAM const);
	virtual void OnSize       (WPARAM const, LPARAM const);
	virtual void OnPaint();

	void newFieldSize(PIXEL const, GridPoint const);
    bool inObservedClientRect(LPARAM const);
    void moveGrid(PixelPoint const);
};
