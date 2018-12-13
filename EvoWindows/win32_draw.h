// win32_draw.h : 
//

#pragma once

#include <array>
#include <iostream>     
#include <sstream> 
#include "pixelRect.h"
#include "gridPoint.h"
#include "gridRect.h"
#include "kGridPoint.h"
#include "win32_util.h"
#include "win32_clut.h"

//lint -esym( 763, EvolutionCore )  redundant declaration

class EvolutionCore;
class PixelCoordinates;
class GridPointShape;
class ColorManager;
class DspOptWindow;
class TextDisplay;
class D3dBuffer;
class Shape;

using namespace std;

class DrawFrame
{
public:

    DrawFrame( HWND const, EvolutionCore *, PixelCoordinates *, DspOptWindow *, ColorManager * );
    ~DrawFrame( );

    void ResizeDrawFrame( PixelPoint const );
    void DoPaint( HWND, KGridRect const & );
    void SetStripMode( tBoolOp );
	bool SetHighlightPos( PixelPoint const );
	void HighlightShape( Shape const *, GridPoint const );
	void CallStrategyColorDialog( HWND const, tStrategyId const );
	void CallHighlightColorDialog( HWND const );
	void CallSelectionColorDialog( HWND const );
	void AddContextMenuEntries( HMENU const, POINT const );

private:
    DrawFrame             ( DrawFrame const & );  // noncopyable class 
    DrawFrame & operator= ( DrawFrame const & );  // noncopyable class 

    EvolutionCore    * const m_pCore;
    PixelCoordinates * const m_pPixelCoordinates;
    DspOptWindow     * const m_pDspOptWindow;
	ColorManager     * const m_pColorManager;  

    D3dBuffer        * m_pD3dBuffer;
	GridPointShape   * m_gridPointShape;
	TextDisplay      * m_pTextDisplay;
	Shape const      * m_pShapeHighlight;
	GridPoint          m_gpHighlight;

    static UINT const MAX_BG_COLOR = 255;

    CLUT m_clutBackground;

    wostringstream m_wBuffer;

    void startOutputString( )  
	{ 
		m_wBuffer.str( wstring() );
		m_wBuffer.clear();
	}

    COLORREF getBackgroundColor( int const ) const;
    void     setIndividualColor( GridPoint const, float const ) const;
	void     addPrimitive( GridPoint const, COLORREF const, float const ) const;
	
	void prepareGridPoint( GridPoint const );

	void drawBackground( );
    void drawText       ( GridRect  const & );
    void drawIndividuals( GridRect  const & );
    void drawPOI        ( GridPoint const );
};
