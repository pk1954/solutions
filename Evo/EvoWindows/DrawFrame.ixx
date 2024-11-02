// DrawFrame.ixx 
//
// EvoWindows

export module DrawFrame;

import std;
import Types;
import BoolOp;
import ColorLUT;
import WinBasics;
import TextDisplay;
import EvoCoreLib;
import ShapeLib;
import EvoReadBuffer;
import D3D_driver;
import ColorManager;
import DspOptWindow;

export class DrawFrame
{
public:

	DrawFrame();
	~DrawFrame();

    void Start
	(
		HWND             const, 
		EvoReadBuffer  * const, 
		EvoPixelCoords * const, 
		D3D_driver     * const,
		DspOptWindow   * const, 
		ColorManager   * const
	);

    void ResizeDrawFrame(EvolutionCore const * const);
    void DoPaint(EvolutionCore const *);
	void SetStripMode(tBoolOp);
	bool SetHighlightPos(EvolutionCore const * const, PixelPoint const);
	void HighlightShape(Shape const *, GridPoint const);
	void CallStrategyColorDialog(HWND const, Strategy::Id const);
	void CallHighlightColorDialog(HWND const);
	void CallSelectionColorDialog(HWND const);
	void AddContextMenuEntries(EvolutionCore const * const, HMENU const, PixelPoint const);

private:
    DrawFrame             (DrawFrame const &);  // noncopyable class 
    DrawFrame & operator= (DrawFrame const &);  // noncopyable class 

	HWND             m_hwnd;
	GridPoint        m_gpHighlight;
	TextDisplay      m_TextDisplay;
	GridPointShape   m_GridPointShape;
	
	EvoReadBuffer  * m_pReadBuffer;
    EvoPixelCoords * m_pEvoPixelCoords;
    DspOptWindow   * m_pDspOptWindow;
	ColorManager   * m_pColorManager;  
    D3D_driver     * m_pGraphics;
	Shape const    * m_pShapeHighlight;

	static ColIndex const MAX_BG_COLOR()
	{
		static ColIndex constexpr res(255);
		return res;
	}

    ColorLUT m_clutBackground;

    std::wostringstream m_wBuffer;

    void startOutputString()  
	{ 
		m_wBuffer.str(std::wstring());
		m_wBuffer.clear();
	}

    COLORREF getBackgroundColor(ColIndex) const;
    void     setIndividualColor(EvolutionCore const * const, GridPoint const, float const) const;
	void     addPrimitive(GridPoint const, COLORREF const, float const) const;
	
	void drawBackground (EvolutionCore const * const);
    void drawText       (EvolutionCore const * const, GridRect  const &);
    void drawIndividuals(EvolutionCore const * const, GridRect  const &);
};
