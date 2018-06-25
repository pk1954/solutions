// win32_draw.cpp
//

#include "stdafx.h"
#include <iomanip>     
#include <iostream>     
#include <sstream> 
#include <vector>
#include "config.h"
#include "EvolutionModelData.h"
#include "EvolutionCore.h"
#include "plannedActivity.h"
#include "pixelCoordinates.h"
#include "win32_util.h"
#include "d3d_buffer.h"
#include "win32_displayOptions.h"
#include "win32_draw.h"

using namespace std;

static COLORREF const CLR_BLACK = D3DCOLOR_ARGB( 255,   0,   0,   0 );
static COLORREF const CLR_WHITE = D3DCOLOR_ARGB( 255, 255, 255, 255 );
static COLORREF const CLR_GREY  = D3DCOLOR_ARGB( 128, 128, 128, 128 );

DrawFrame::DrawFrame
( 
    HWND                 const hWnd, 
    EvolutionCore      * const pCore,
    EvolutionModelData * const pModel,
    PixelCoordinates   * const pPixelCoordinates, 
    DspOptWindow       * const pDspOptWindow 
) : 
    m_hWnd( hWnd ),
    m_bDimmIndividuals( TRUE ),
    m_pCore( pCore ),
    m_pModelWork( pModel ),
    m_pPixelCoordinates( pPixelCoordinates ),
    m_pDspOptWindow( pDspOptWindow ),
    m_pD3dBuffer( nullptr ), 
    m_clutBackground( )
{
    UINT const uiClutSize = static_cast<UINT>(Config::GetConfigValue( Config::tId::stdCapacity ));
    for ( auto & strategy : m_aClutStrat )
        strategy.Allocate( uiClutSize );

    m_aClutStrat[ static_cast<int>( tStrategyId::defectAlways )   ].SetColorHi( RGB(151, 171, 255) );
    m_aClutStrat[ static_cast<int>( tStrategyId::cooperateAlways )].SetColorHi( RGB(127, 255,   0) );
    m_aClutStrat[ static_cast<int>( tStrategyId::tit4tat )        ].SetColorHi( RGB(255,  50,  50) );

    m_pD3dBuffer = new D3dBuffer( hWnd, GridPoint::GRID_AREA );
	m_clutBackground.Allocate( MAX_BG_COLOR );    // default is grey scale lookup table with entries 0 .. 255
    SetIndDimmMode( Config::GetConfigValueBoolOp(Config::tId::dimmMode ) );
}

DrawFrame::~DrawFrame( ) 
{ 
    delete m_pD3dBuffer;
    m_pD3dBuffer = nullptr;
};

void DrawFrame::SetIndDimmMode( tBoolOp const bOp )
{
    ApplyOp( m_bDimmIndividuals, bOp );

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
	int const MAX_TEXT_LINES = 20;
	int       iFontSize = m_pPixelCoordinates->GetFieldSize() / MAX_TEXT_LINES;
	if ( iFontSize < 9 )
		iFontSize = 9;
	if ( iFontSize > 16 )
		iFontSize = 16;
    m_pD3dBuffer->ResetFont( iFontSize );   
}

void DrawFrame::DoPaint( KGridRect const & pkgr )
{
    if ( IsWindowVisible( m_hWnd ) )
    {
		m_pD3dBuffer->StartFrame( );

		{
			float m_fPxSize = static_cast<float>( m_pPixelCoordinates->GetFieldSize( ) );
			Apply2Grid
			( 
    			[&](GridPoint const & gp, short const s)
				{
					int   const iValue  = m_pDspOptWindow->GetIntValue( gp );
					DWORD const dwColor = getBackgroundColor( iValue );
					m_pD3dBuffer->AddBackgroundPrimitive( m_pPixelCoordinates->Grid2PixelPosCenter( gp ), dwColor, m_fPxSize );
				}
			);
		}
        m_pD3dBuffer->RenderBackground( );

        if ( m_pDspOptWindow->AreIndividualsVisible( ) )
        {
            GridRect        rcGrid( m_pPixelCoordinates->Pixel2GridRect( Util::GetClPixelRect( m_hWnd ) ) );
            GridPoint const gpPoi = m_pCore->FindPOI( m_pModelWork );
            drawPOI( gpPoi );
            rcGrid.ClipToGrid( );
            drawIndividuals( rcGrid );
            m_pD3dBuffer->RenderIndividuals( ); 

            if ( m_pPixelCoordinates->GetFieldSize() >= 96 )
                drawText( rcGrid, gpPoi );
        }

        if ( m_pModelWork->SelectionIsNotEmpty() )
            m_pD3dBuffer->RenderTranspRect( m_pPixelCoordinates->Grid2PixelRect( m_pModelWork->GetSelection() ), D3DCOLOR_ARGB( 64, 0, 217, 255)  );  

        if ( pkgr.IsNotEmpty( ) )
            m_pD3dBuffer->RenderTranspRect( m_pPixelCoordinates->KGrid2PixelRect( pkgr ), D3DCOLOR_ARGB( 128, 255, 217, 0) );  

        m_pD3dBuffer->EndFrame( );  
    }
}

void DrawFrame::drawPOI( GridPoint const & gpPoi )
{
    if ( gpPoi.IsNotNull( ) )
    {
        PixelPoint const ptCenter = m_pPixelCoordinates->Grid2PixelPosCenter( gpPoi );
		float      const fPixSize = static_cast<float>( m_pPixelCoordinates->GetFieldSize( ) );

        m_pD3dBuffer->AddIndividualPrimitive( ptCenter, CLR_WHITE, fPixSize * 0.50f );   // white frame for POI
        m_pD3dBuffer->AddIndividualPrimitive( ptCenter, CLR_BLACK, fPixSize * 0.45f );   // black frame for POI

        PlannedActivity const & planPoi = m_pCore->GetPlan( );
        if ( planPoi.IsValid( ) )
        {
            GridPoint const gpTarget = planPoi.GetTarget( );
            if ( gpTarget.IsNotNull( ) )
                m_pD3dBuffer->AddIndividualPrimitive( m_pPixelCoordinates->Grid2PixelPosCenter( gpTarget ), CLR_GREY, fPixSize * 0.45f );   // mark target

            GridPoint const gpPartner = planPoi.GetPartner( );
            if ( gpPartner.IsNotNull( ) )
                m_pD3dBuffer->AddIndividualPrimitive( m_pPixelCoordinates->Grid2PixelPosCenter( gpPartner ), CLR_GREY, fPixSize * 0.45f );   // mark target
        }
    }
}

void DrawFrame::drawIndividuals( GridRect const & rect  )
{
    short const sFieldSize   = m_pPixelCoordinates->GetFieldSize();
    long  const lHalfSizeInd = ( sFieldSize <    8 ) ?                         1  :
                               ( sFieldSize <=  16 ) ? ((3 * sFieldSize) / 8 - 1) : 
                                                       ((3 * sFieldSize) / 8    );

    Apply2Rect
	( 
		[&](GridPoint const & gp, short const s) 
		{ 
			setIndividualColor( gp, static_cast<float>( lHalfSizeInd ) ); 
		}, 
		rect
	);
/*
#ifndef NDEBUG
    if ( rect == GridRect::GRID_RECT_FULL )
    {
        m_pCore->DumpGridPointList( m_pModelWork );
    }
#endif
*/
}

COLORREF DrawFrame::getTextColor( GridPoint const & gp ) const
{
	return CLR_WHITE;
}

void DrawFrame::drawText( GridRect const & rect, GridPoint const & gpPoi )
{
    short const sFieldSize   = m_pPixelCoordinates->GetFieldSize();
    long  const lHalfSizeInd = (5 * sFieldSize) / 16;

    Apply2Rect
	( 
		[&](GridPoint const & gp, short const s)
		{
            if ( GetEvoCore( )->IsAlive( gp ) )
            {
				long       lHeight    = Util::GetClientWindowHeight( m_hWnd );
				COLORREF   colText    = getTextColor( gp );
                PixelPoint ptCenter   = m_pPixelCoordinates->Grid2PixelPosCenter( gp );
						   Util::UpsideDown( m_hWnd, & ptCenter ); 
                PixelRect  pixRect ( ptCenter  - lHalfSizeInd, ptCenter  + lHalfSizeInd );

                assembleLeftColumn( gp, gpPoi );
                m_pD3dBuffer->D3D_DrawText( pixRect, getOutputString( ), colText );

                if ( sFieldSize >= 256 ) 
                {
                    pixRect.left += lHalfSizeInd;
                    assembleRightColumn( gp );
                    m_pD3dBuffer->D3D_DrawText( pixRect, getOutputString( ), colText );
                }
            }
		},
		rect
	);
}

void DrawFrame::assembleLeftColumn( GridPoint const & gp, GridPoint const & gpPoi )
{
    startOutputString( );

	m_wBuffer << gp                                                                     << endl;
    m_wBuffer << L"ID: " << setw( 5 ) << m_pModelWork->GetId( gp )                      << endl;
    m_wBuffer << L"En: " << setw( 5 ) << m_pModelWork->GetEnergy( gp )                  << endl;
    m_wBuffer << L"Age:" << setw( 5 ) << m_pModelWork->GetAge( gp )                     << endl;
    m_wBuffer << L"Or: " << setw( 5 ) << GetOriginName( m_pModelWork->GetOrigin( gp ) ) << endl;

    PlannedActivity plan = m_pCore->GetPlan( );
    if ( plan.IsValid( ) )
    {
        if ( (gp == plan.GetActor( )) || (gp == plan.GetPartner( )) )
        {
            m_wBuffer << (( gp == plan.GetActor( ) ) ? L"** ACTOR **" : L"** PARTNER **") << endl;
            m_wBuffer << L"** "  << GetActionTypeName( plan.GetActionType( ) ) << L" **"  << endl;
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
        
        m_wBuffer << L"  Mem " << memFilled << L"/" << memSize << endl;

        for	( MEM_INDEX mem = 0; mem < memFilled; ++mem )
        {
            m_wBuffer << setw( 10 ) << m_pModelWork->GetMemEntry( gp, mem ) << endl;
        }
    }

    finishOutputString( );
}

void DrawFrame::setIndividualColor( GridPoint const & gp, float const fHalfSize ) const
{
    tStrategyId const strat   = m_pModelWork->GetStrategyId( gp );
    short       const sEnergy = m_pModelWork->GetEnergy  ( gp );
    if ( sEnergy <= 0 )
        return;
    if ( static_cast<int>( strat ) >= NR_STRATEGIES )
        return;                   // can happen in case of race conditions between display thread and worker thread
    //lint -e571  suspicious cast
    UINT const uiIndex = static_cast<UINT>( sEnergy );
    //lint +e571
    CLUT const & clut = m_aClutStrat.at( static_cast<int>( strat ) );
    assert( uiIndex < clut.GetSize() ); 
    m_pD3dBuffer->AddIndividualPrimitive
	( 
		m_pPixelCoordinates->Grid2PixelPosCenter( gp ), 
		clut.Get( uiIndex ), 
		fHalfSize 
	);
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
