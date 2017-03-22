// win32_draw.cpp
//

#include "stdafx.h"
#include <iostream>     
#include <sstream> 
#include <vector>
#include "config.h"
#include "EvolutionModelData.h"
#include "EvolutionCore.h"
#include "plannedActivity.h"
#include "win32_util.h"
#include "win32_frameBuffer.h"
#include "d3d_buffer.h"
#include "win32_displayOptions.h"
#include "win32_draw.h"

using namespace std;

//lint -esym( 768, DrawFrame::drawGridPointFunc )     symbol not referenced
//lint -esym( 768, DrawFrame::drawBackground )        symbol not referenced
//lint -e845                                          0 as right argument to << or |

//// The family of drawGridPointFunc classes provides functors of GridPoint_Functor type
//// which have access to EvolutionCore, DrawFrame, FrameBuffer and D3DBuffer objects

class DrawFrame::drawGridPointFunc : public GridPoint_Functor
{
protected:
    explicit drawGridPointFunc( DrawFrame * const pDraw ) :
        GridPoint_Functor( ),
        m_pDraw       ( pDraw ), 
        m_pFrameBuffer( m_pDraw->m_pFrameBuffer ) ,
        m_pD3dBuffer  ( m_pDraw->m_pD3dBuffer )
    { };

    DrawFrame   * m_pDraw;
    FrameBuffer * m_pFrameBuffer;
    D3dBuffer   * m_pD3dBuffer;
};

class DrawFrame::drawBackground : public drawGridPointFunc
{
public:
    drawBackground
    ( 
        EvolutionModelData          const *       pModel,
        DrawFrame                * const pDraw, 
        GetIntValueFunctor const &       getIntFunctor
    ) :
        drawGridPointFunc( pDraw ),
        m_fPxSize( static_cast<float>( pDraw->m_pFrameBuffer->GetFieldSize( ) ) ),
        m_pGetIntFunctor( & getIntFunctor )
    { };

    virtual void operator() ( GridPoint const & gp )
    {
        int   const iValue  = ( * m_pGetIntFunctor )( gp );
        DWORD const dwColor = m_pDraw->getBackgroundColor( iValue );
        m_pD3dBuffer->AddBackgroundRect( m_pFrameBuffer->Grid2PixelPos( gp ), dwColor, m_fPxSize );
    }

private:
    float                      m_fPxSize;
    GetIntValueFunctor const * m_pGetIntFunctor;
};

/////////////////////////////////////

DrawFrame::DrawFrame
( 
    HWND            const hWnd, 
    EvolutionCore * const pCore,
    EvolutionModelData     * const pModel,
    FrameBuffer   * const pFrameBuffer, 
    DspOptWindow  * const pDspOptWindow, 
    GridRect      * const pGridRectSel
) : 
    m_hWnd( hWnd ),
    m_bDimmIndividuals( TRUE ),
    m_pCore( pCore ),
    m_pModelWork( pModel ),
    m_pFrameBuffer( pFrameBuffer ),
    m_pDspOptWindow( pDspOptWindow ),
    m_pGridRectSel( pGridRectSel ),
    m_pD3dBuffer( new D3dBuffer( hWnd, GridPoint::GRID_AREA ) ),
    m_clutBackground( )
{
    m_clutBackground.Allocate( MAX_BG_COLOR );    // default is grey scale lookup table with entries 0 .. 255
    
    UINT const uiClutSize = static_cast<UINT>(Config::GetConfigValue( Config::tId::stdCapacity ));
    for ( auto &strategy : m_aClutStrat )
        strategy.Allocate( uiClutSize );

    SetIndDimmMode( tBoolOp::opFalse );

    m_aClutStrat[ static_cast<int>( tStrategyId::defectAlways )   ].SetColorHi( RGB(151, 171, 255) );
    m_aClutStrat[ static_cast<int>( tStrategyId::cooperateAlways )].SetColorHi( RGB(127, 255,   0) );
    m_aClutStrat[ static_cast<int>( tStrategyId::tit4tat )        ].SetColorHi( RGB(255,  50,  50) );
}

DrawFrame::~DrawFrame( ) 
{ 
    delete m_pD3dBuffer;
    m_pD3dBuffer = nullptr;
};

void DrawFrame::SetIndDimmMode( tBoolOp const bOp )
{
    Util::ApplyOp( m_bDimmIndividuals, bOp );

    UINT const uiClutBase = m_bDimmIndividuals // color of individuals ...
                          ? 30       // ... varies from 30% - 100%, depending on energy 
                          : 100;     // ... is always at 100%

    for ( auto &strategy : m_aClutStrat )
        strategy.SetClutBase( uiClutBase );
}

void DrawFrame::SetStripMode( tBoolOp const bOp ) 
{ 
    m_pD3dBuffer->SetStripMode( bOp ); 
};

void DrawFrame::Resize( )
{
    m_pD3dBuffer->ResetFont( );
}

void DrawFrame::DoPaint( KGridRect const &pkgr )
{
    if ( IsWindowVisible( m_hWnd ) )
    {
        m_pD3dBuffer->StartFrame( );

        Apply2Grid( & drawBackground( m_pModelWork, this, m_pDspOptWindow->GetDisplayFunctor( ) ) );
        m_pD3dBuffer->RenderVertices( );

        if ( m_pDspOptWindow->AreIndividualsVisible( ) )
        {
            GridRect rcGrid( m_pFrameBuffer->Pixel2GridRect( Util::GetClRect( m_hWnd ) ) );

            rcGrid.ClipToGrid( );

            GridPoint const gpPoi = m_pCore->FindPOI( m_pModelWork );
            drawPOI( gpPoi );
            drawIndividuals( rcGrid );
            m_pD3dBuffer->RenderRects( ); 

            if ( m_pFrameBuffer->GetFieldSize() >= 128 )
                drawText( rcGrid, gpPoi );
        }

        if ( m_pGridRectSel->IsNotEmpty() )
            m_pD3dBuffer->RenderTranspRect( m_pFrameBuffer->Grid2PixelRect( * m_pGridRectSel ), D3DCOLOR_ARGB( 64, 0, 217, 255)  );  

        if ( pkgr.IsNotEmpty( ) )
            m_pD3dBuffer->RenderTranspRect( m_pFrameBuffer->KGrid2PixelRect( pkgr ), D3DCOLOR_ARGB( 128, 255, 217, 0) );  

//        if ( m_pDspOptWindow-> )            //TODO: display editor cursor
//            drawTransparentCone( );

        m_pD3dBuffer->EndFrame( );  
    }
}

void DrawFrame::drawPOI( GridPoint const & gpPoi )
{
    static COLORREF const CLR_BLACK = D3DCOLOR_ARGB( 255,   0,   0,   0 );
    static COLORREF const CLR_WHITE = D3DCOLOR_ARGB( 255, 255, 255, 255 );
    static COLORREF const CLR_GREY  = D3DCOLOR_ARGB( 128, 128, 128, 128 );

    if ( gpPoi.IsNotNull( ) )
    {
        PixelPoint const ptCenter = m_pFrameBuffer->Grid2PixelPosCenter( gpPoi );

        m_pD3dBuffer->AddRect( ptCenter, CLR_WHITE, static_cast<float>(m_pFrameBuffer->GetFieldSize()) * 0.50f );   // white frame for POI
        m_pD3dBuffer->AddRect( ptCenter, CLR_BLACK, static_cast<float>(m_pFrameBuffer->GetFieldSize()) * 0.45f );   // black frame for POI

        PlannedActivity const & planPoi = m_pCore->GetPlan( );
        if ( planPoi.IsValid( ) )
        {
            GridPoint const gpTarget = planPoi.GetTarget( );
            if ( gpTarget.IsNotNull( ) )
                m_pD3dBuffer->AddRect( m_pFrameBuffer->Grid2PixelPosCenter( gpTarget ), CLR_GREY, static_cast<float>( m_pFrameBuffer->GetFieldSize( ) ) * 0.45f );   // mark target

            GridPoint const gpPartner = planPoi.GetPartner( );
            if ( gpPartner.IsNotNull( ) )
                m_pD3dBuffer->AddRect( m_pFrameBuffer->Grid2PixelPosCenter( gpPartner ), CLR_GREY, static_cast<float>( m_pFrameBuffer->GetFieldSize( ) ) * 0.45f );   // mark target
        }
    }
}

void DrawFrame::drawIndividuals( GridRect const & rect  )
{
    class draw : public drawGridPointFunc
    {
    public:
        draw( DrawFrame * const pDraw, float const fHalfSizeInd ) :   
            drawGridPointFunc( pDraw ),
            m_fHalfSizeInd( fHalfSizeInd )
        { };

        virtual void operator() ( GridPoint const & gp )
        {
            COLORREF color;
            if ( m_pDraw->getIndividualColor( gp, color ) )
                m_pD3dBuffer->AddRect( m_pFrameBuffer->Grid2PixelPosCenter( gp ), color, m_fHalfSizeInd );
        }

    private:
        float m_fHalfSizeInd;
    };

    short const sFieldSize   = m_pFrameBuffer->GetFieldSize();
    long  const lHalfSizeInd = ( sFieldSize <    8 ) ?                         1  :
                               ( sFieldSize <=  16 ) ? ((3 * sFieldSize) / 8 - 1) : 
                                                       ((3 * sFieldSize) / 8    );
    Apply2Rect( & draw( this, static_cast<float>( lHalfSizeInd ) ), rect );
#ifndef NDEBUG
    if ( rect == GridRect::GRID_RECT_FULL )
    {
        m_pCore->DumpGridPointList( );
    }
#endif
}

void DrawFrame::drawText( GridRect const & rect, GridPoint const & gpPoi )
{
    class draw : public drawGridPointFunc
    {
    public:
        explicit draw( DrawFrame * const pDraw, GridPoint const & gpPoi ) :
            drawGridPointFunc( pDraw ),
            m_lHeight   ( Util::GetClientWindowHeight( m_pDraw->m_hWnd ) ),
            m_sFieldSize( m_pFrameBuffer->GetFieldSize() ),
            m_gpPoi( gpPoi )
        { };

        virtual void operator() ( GridPoint const & gp )
        {
            if ( m_pDraw->GetEvoCore( )->IsAlive( gp ) )
            {
                PixelPoint ptCenter     = m_pFrameBuffer->Grid2PixelPosCenter( gp );
                           ptCenter.y   = m_lHeight - ptCenter.y;            // for DirectX text output
                long const lHalfSizeInd = (5 * m_sFieldSize) / 16;
                PixelRect  pixRect( ptCenter - lHalfSizeInd, ptCenter + lHalfSizeInd );
               
                m_pDraw->assembleLeftColumn( gp, m_gpPoi );
                m_pD3dBuffer->D3D_DrawText( pixRect, m_pDraw->getOutputString( ) );

                if ( m_sFieldSize >= 256 ) 
                {
                    pixRect.left += lHalfSizeInd;
                    m_pDraw->assembleRightColumn( gp );
                    m_pD3dBuffer->D3D_DrawText( pixRect, m_pDraw->getOutputString( ) );
                }
            }
        }

    private:
        long      m_lHeight;
        short     m_sFieldSize;
        GridPoint m_gpPoi;
    };

    Apply2Rect( & draw( this, gpPoi ), rect );
}

void DrawFrame::assembleLeftColumn( GridPoint const & gp, GridPoint const & gpPoi )
{
    startOutputString( );

    m_wBuffer << gp;
    if ( gp == gpPoi )
        m_wBuffer << L" ** POI ** ";
    m_wBuffer << endl;
    m_wBuffer << L"ID: "     << m_pModelWork->GetId( gp )                      << endl;
    m_wBuffer << L"Age: "    << m_pModelWork->GetAge( gp )                     << endl;
    m_wBuffer << L"Origin: " << GetOriginName( m_pModelWork->GetOrigin( gp ) ) << endl;
    m_wBuffer << L"Energy: " << m_pModelWork->GetEnergy( gp )                  << endl;

    PlannedActivity plan = m_pCore->GetPlan( );
    if ( plan.IsValid( ) )
    {
        if ( (gp == plan.GetActor( )) || (gp == plan.GetPartner( )) )
        {
            m_wBuffer << (( gp == plan.GetActor( ) ) ? L"** ACTOR **" : L"** PARTNER **") << endl;
            m_wBuffer << L"Action: "    << GetActionTypeName( plan.GetActionType( ) )     << endl;
            m_wBuffer << L"BaseCons: "  << plan.GetBaseConsumption( )                     << endl;
        }
    }

    finishOutputString( );
}

void DrawFrame::assembleRightColumn( GridPoint const & gp )
{
    startOutputString( );

    if ( m_pModelWork->GetStrategyId( gp ) == tStrategyId::tit4tat )
    {
        MEM_INDEX const memSize   = m_pModelWork->GetMemSize( gp );  
        MEM_INDEX const memFilled = m_pModelWork->GetMemUsed( gp ); 
        
        m_wBuffer << L"Memory (" << memFilled << L" of " << memSize << L")" << endl;

        for	( MEM_INDEX mem = 0; mem < memFilled; ++mem )
        {
            m_wBuffer << m_pModelWork->GetMemEntry( gp, mem ) << endl;
        }
    }

    finishOutputString( );
}

BOOL DrawFrame::getIndividualColor( GridPoint const & gp, COLORREF & color ) const
{
    tStrategyId const strat   = m_pModelWork->GetStrategyId( gp );
    short       const sEnergy = m_pModelWork->GetEnergy  ( gp );
    if ( sEnergy <= 0 )
        return FALSE;
    if ( static_cast<int>( strat ) >= NR_STRATEGIES )
        return FALSE;                   // can happen in case of race conditions between display thread and worker thread
    //lint -e571  suspicious cast
    UINT const uiIndex = static_cast<UINT>( sEnergy );
    //lint +e571
    CLUT const & clut = m_aClutStrat.at( static_cast<int>( strat ) );
    assert( uiIndex < clut.GetSize() ); 
    color = clut.Get( uiIndex );
    return TRUE;
}

COLORREF DrawFrame::getBackgroundColor( int const iValue ) const
{
    unsigned int uiIndex;

    if ( iValue < 0 )
    {
        uiIndex = 0;
    }
    else
    {
        uiIndex = static_cast<unsigned int>( iValue );
        if ( uiIndex > MAX_BG_COLOR )
            uiIndex = MAX_BG_COLOR;
    }

    return m_clutBackground.Get( uiIndex );
}
