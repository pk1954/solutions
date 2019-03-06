// win32_draw.h : 
//
// EvoWindows

#pragma once

#include <array>
#include <iostream>     
#include <sstream> 
#include "PixelTypes.h"
#include "gridPoint.h"
#include "gridRect.h"
#include "win32_util.h"
#include "win32_clut.h"

//lint -esym( 763, EvolutionCore )  redundant declaration

class EvolutionCore;
class GraphicsInterface;
class PixelCoordinates;
class GridPointShape;
class ColorManager;
class DspOptWindow;
class TextDisplay;
class Shape;

class DrawFrame
{
public:

    DrawFrame
	( 
		HWND const, 
		EvolutionCore * const, 
		PixelCoordinates * const, 
		GraphicsInterface * const,
		DspOptWindow * const, 
		ColorManager * const
	);
    ~DrawFrame( );

    void ResizeDrawFrame( );
    void DoPaint( HWND, PixelRect const & );
    void SetStripMode( tBoolOp );
	bool SetHighlightPos( PixelPoint const );
	void HighlightShape( Shape const *, GridPoint const );
	void CallStrategyColorDialog( HWND const, Strategy::Id const );
	void CallHighlightColorDialog( HWND const );
	void CallSelectionColorDialog( HWND const );
	void AddContextMenuEntries( HMENU const, POINT const );

private:
    DrawFrame             ( DrawFrame const & );  // noncopyable class 
    DrawFrame & operator= ( DrawFrame const & );  // noncopyable class 

    EvolutionCore     * const m_pCore;
    PixelCoordinates  * const m_pPixelCoordinates;
    DspOptWindow      * const m_pDspOptWindow;
	ColorManager      * const m_pColorManager;  

    GraphicsInterface * m_pGraphics;
	GridPointShape    * m_gridPointShape;
	TextDisplay       * m_pTextDisplay;
	Shape const       * m_pShapeHighlight;
	GridPoint           m_gpHighlight;

	static CLUT_INDEX const MAX_BG_COLOR()
	{
		static const CLUT_INDEX res(255);
		return res;
	}

    CLUT m_clutBackground;

    std::wostringstream m_wBuffer;

    void startOutputString( )  
	{ 
		m_wBuffer.str( std::wstring() );
		m_wBuffer.clear();
	}

    COLORREF getBackgroundColor( CLUT_INDEX ) const;
    void     setIndividualColor( GridPoint const, float const ) const;
	void     addPrimitive( GridPoint const, COLORREF const, float const ) const;
	
	void drawBackground( );
    void drawText       ( GridRect  const & );
    void drawIndividuals( GridRect  const & );
    void drawPOI        ( GridPoint const );
};
