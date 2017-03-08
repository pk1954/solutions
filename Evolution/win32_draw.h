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
class ModelData;
class FrameBuffer;
class DspOptWindow;
class D3dBuffer;

class DrawFrame
{
public:

    DrawFrame( HWND, EvolutionCore *, ModelData *, FrameBuffer *, DspOptWindow *, GridRect * );
    ~DrawFrame( );

    void Resize( );
    void DoPaint( KGridRect const & );
    void SetStripMode( tBoolOp );
    void SetIndDimmMode( tBoolOp );

    ModelData * const GetEvoCore( ) const { return m_pModelWork; } // for inner class

private:
    DrawFrame             ( DrawFrame const & );  // noncopyable class 
    DrawFrame & operator= ( DrawFrame const & );  // noncopyable class 

    class drawGridPointFunc;
    class drawBackground;

    HWND const m_hWnd;
    BOOL       m_bDimmIndividuals;   

    EvolutionCore  * const m_pCore;
    ModelData      * const m_pModelWork;
    FrameBuffer    * const m_pFrameBuffer;
    DspOptWindow   * const m_pDspOptWindow;
    GridRect const * const m_pGridRectSel;
    D3dBuffer      *       m_pD3dBuffer;

    static UINT const MAX_BG_COLOR = 255;

                CLUT                                                  m_clutBackground;
    std::array< CLUT, NR_STRATEGIES> m_aClutStrat;

    std::wstring        m_wString;
    std::wostringstream m_wBuffer;

    void startOutputString( )  { m_wBuffer.str( L"" ); }
    void finishOutputString( ) { m_wString = m_wBuffer.str( ); }

    std::wstring const & getOutputString( ) { return m_wString; }

    COLORREF getBackgroundColor ( int const ) const;
    BOOL     getIndividualColor ( GridPoint const &, COLORREF & ) const;
    void     assembleLeftColumn ( GridPoint const &, GridPoint const & );
    void     assembleRightColumn( GridPoint const & );

    void drawPOI        ( GridPoint const & );
    void drawIndividuals( GridRect const & );
    void drawText       ( GridRect const &, GridPoint const & );
//    void drawTransparentCone( );
};
