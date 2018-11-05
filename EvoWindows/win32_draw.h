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
class IndividualShape;
class DspOptWindow;
class TextDisplay;
class D3dBuffer;
class GridPointShape;

using namespace std;

class DrawFrame
{
public:

    DrawFrame( EvolutionCore *, PixelCoordinates *, DspOptWindow * );
    ~DrawFrame( );

    void Resize( );
    void DoPaint( HWND, KGridRect const & );
	void SetStrategyColor( tStrategyId const, COLORREF const );
    void SetStripMode( tBoolOp );
    void SetIndDimmMode( tBoolOp );
	bool SetHighlightPos( PixelPoint const );
	void HighlightRect( PixelRect const & );

private:
    DrawFrame             ( DrawFrame const & );  // noncopyable class 
    DrawFrame & operator= ( DrawFrame const & );  // noncopyable class 

    bool m_bDimmIndividuals;   

    EvolutionCore    * const m_pCore;
    PixelCoordinates * const m_pPixelCoordinates;
    DspOptWindow     * const m_pDspOptWindow;

    D3dBuffer        * m_pD3dBuffer;
	IndividualShape  * m_pIndividualShape;
	TextDisplay      * m_pTextDisplay;

	GridPointShape const * m_pShapeHighlight;
	GridPoint              m_gpHighlight;

    static UINT const MAX_BG_COLOR = 255;

           CLUT                 m_clutBackground;
    array< CLUT, NR_STRATEGIES> m_aClutStrat;

    wostringstream m_wBuffer;

    void startOutputString( )  
	{ 
		m_wBuffer.str( wstring() );
		m_wBuffer.clear();
	}

    COLORREF getBackgroundColor( int const ) const;
    void     setIndividualColor( GridPoint const, float const ) const;
	void     addPrimitive( GridPoint const, DWORD const, float const ) const;
	
	void prepareGridPoint( GridPoint const );

	void drawBackground( );
    void drawText       ( GridRect  const & );
    void drawIndividuals( GridRect  const & );
    void drawPOI        ( GridPoint const );
};
