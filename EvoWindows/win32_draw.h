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
class EvolutionModelData;
class PixelCoordinates;
class DspOptWindow;
class D3dBuffer;

class DrawFrame
{
public:

    DrawFrame( HWND, EvolutionCore *, EvolutionModelData *, PixelCoordinates *, DspOptWindow * );
    ~DrawFrame( );

    void Resize( );
    void DoPaint( KGridRect const & );
    void SetStripMode( tBoolOp );
    void SetIndDimmMode( tBoolOp );

    EvolutionModelData * const GetEvoCore( ) const { return m_pModelWork; } // for inner class

private:
    DrawFrame             ( DrawFrame const & );  // noncopyable class 
    DrawFrame & operator= ( DrawFrame const & );  // noncopyable class 

    HWND const m_hWnd;
    bool       m_bDimmIndividuals;   

    EvolutionCore      * const m_pCore;
    EvolutionModelData * const m_pModelWork;
    PixelCoordinates   * const m_pPixelCoordinates;
    DspOptWindow       * const m_pDspOptWindow;
    D3dBuffer          *       m_pD3dBuffer;

    static UINT const MAX_BG_COLOR = 255;

                CLUT                 m_clutBackground;
    std::array< CLUT, NR_STRATEGIES> m_aClutStrat;

    std::wstring        m_wString;
    std::wostringstream m_wBuffer;

    void startOutputString( )  { m_wBuffer.str( L"" ); }
    void finishOutputString( ) { m_wString = m_wBuffer.str( ); }

    std::wstring const & getOutputString( ) { return m_wString; }

    COLORREF getBackgroundColor ( int const ) const;
    COLORREF getTextColor       ( GridPoint const & ) const;
    void     setIndividualColor ( GridPoint const &, float const ) const;
    void     assembleLeftColumn ( GridPoint const & );
    void     assembleRightColumn( GridPoint const & );

	void drawBackground ( );
    void drawIndividuals( GridRect const & );
    void drawText       ( GridRect const & );
    void drawPOI        ( GridPoint const & );
};
