// win32_draw.h : 
//
// EvoWindows

#pragma once

#include <array>
#include <iostream>     
#include <sstream> 
#include "Strategy.h"
#include "PixelTypes.h"
#include "gridPoint.h"
#include "gridRect.h"
#include "win32_util.h"
#include "win32_clut.h"
#include "win32_gridPointShape.h"
#include "win32_textDisplay.h"

class ReadBuffer;
class EvolutionCore;
class GraphicsInterface;
class PixelCoordinates;
class ColorManager;
class DspOptWindow;
class Shape;

class DrawFrame
{
public:

	DrawFrame();
	~DrawFrame( );

    void Start
	( 
		HWND                const, 
		ReadBuffer        * const, 
		PixelCoordinates  * const, 
		GraphicsInterface * const,
		DspOptWindow      * const, 
		ColorManager      * const
	);

    void ResizeDrawFrame( EvolutionCore const * const );
    void DoPaint( EvolutionCore const * );
    void SetStripMode( tBoolOp );
	bool SetHighlightPos( EvolutionCore const * const, PixelPoint const );
	void HighlightShape( Shape const *, GridPoint const );
	void CallStrategyColorDialog( HWND const, Strategy::Id const );
	void CallHighlightColorDialog( HWND const );
	void CallSelectionColorDialog( HWND const );
	void AddContextMenuEntries( EvolutionCore const * const, HMENU const, POINT const );

private:
    DrawFrame             ( DrawFrame const & );  // noncopyable class 
    DrawFrame & operator= ( DrawFrame const & );  // noncopyable class 

	HWND                m_hwnd;
	GridPoint           m_gpHighlight;
	TextDisplay         m_TextDisplay;
	GridPointShape      m_GridPointShape;
	
	ReadBuffer        * m_pReadBuffer;
    PixelCoordinates  * m_pPixelCoordinates;
    DspOptWindow      * m_pDspOptWindow;
	ColorManager      * m_pColorManager;  
    GraphicsInterface * m_pGraphics;
	Shape const       * m_pShapeHighlight;

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
    void     setIndividualColor( EvolutionCore const * const, GridPoint const, float const ) const;
	void     addPrimitive( GridPoint const, COLORREF const, float const ) const;
	
	void drawBackground( );
    void drawText       ( EvolutionCore const * const, GridRect  const & );
    void drawIndividuals( EvolutionCore const * const, GridRect  const & );
};
