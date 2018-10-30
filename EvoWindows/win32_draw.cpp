// win32_draw.cpp
//

#include "stdafx.h"
#include <vector>
#include "config.h"
#include "EvolutionCore.h"
#include "pixelCoordinates.h"
#include "plannedActivity.h"
#include "win32_util.h"
#include "d3d_buffer.h"
#include "win32_displayOptions.h"
#include "win32_individualShape.h"
#include "win32_draw.h"

using namespace std;

static COLORREF const CLR_DEFECT    = RGB(151, 171, 255);
static COLORREF const CLR_COOPERATE = RGB(127, 255,   0);
static COLORREF const CLR_TIT4TAT   = RGB(255,  50,  50);

DrawFrame::DrawFrame
( 
    EvolutionCore    * const pCore,
    PixelCoordinates * const pPixelCoordinates, 
    DspOptWindow     * const pDspOptWindow 
) : 
    m_bDimmIndividuals( TRUE ),
    m_pCore( pCore ),
    m_pPixelCoordinates( pPixelCoordinates ),
    m_pDspOptWindow( pDspOptWindow ),
    m_pD3dBuffer( nullptr ), 
    m_clutBackground( ),
	m_pIndividualShape( nullptr )
{
    UINT const uiClutSize = static_cast<UINT>(Config::GetConfigValue( Config::tId::stdCapacity ));
    for ( auto & strategy : m_aClutStrat )
        strategy.Allocate( uiClutSize );

    m_pD3dBuffer = new D3dBuffer( GridPoint::GRID_AREA );
	m_clutBackground.Allocate( MAX_BG_COLOR );    // default is grey scale lookup table with entries 0 .. 255
    SetIndDimmMode( Config::GetConfigValueBoolOp(Config::tId::dimmMode ) );

    SetStrategyColor( tStrategyId::defect,    CLR_DEFECT    );
    SetStrategyColor( tStrategyId::cooperate, CLR_COOPERATE );
    SetStrategyColor( tStrategyId::tit4tat,   CLR_TIT4TAT   );

	m_pIndividualShape_Level_1 = new IndividualShape_Level_1( m_pD3dBuffer, m_wBuffer, m_pCore, m_pPixelCoordinates );
	m_pIndividualShape_Level_2 = new IndividualShape_Level_2( m_pD3dBuffer, m_wBuffer, m_pCore, m_pPixelCoordinates );
	Resize();
}

void DrawFrame::SetStrategyColor( tStrategyId const strat, COLORREF const col )
{
    m_aClutStrat[ static_cast<int>(strat) ].SetColorHi( col );
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
	short const sFieldSize     = m_pPixelCoordinates->GetFieldSize();
	int   const MAX_TEXT_LINES = 20;
	int         iFontSize      = sFieldSize / MAX_TEXT_LINES;
	if ( iFontSize < 9 )
		iFontSize = 9;
	if ( iFontSize > 16 )
		iFontSize = 16;
    m_pD3dBuffer->ResetFont( iFontSize );
	m_pIndividualShape = ( sFieldSize < 256 ) ? m_pIndividualShape_Level_1 : m_pIndividualShape_Level_2;
	m_pIndividualShape->Resize( sFieldSize );
}

void DrawFrame::DoPaint( HWND hwnd, KGridRect const & pkgr )
{
    if ( IsWindowVisible( hwnd ) )
    {
		m_pD3dBuffer->StartFrame( hwnd );

		drawBackground( );

        if ( m_pDspOptWindow->AreIndividualsVisible( ) )
        {
			PixelRect const pixRect( Util::GetClPixelRect( hwnd ) );
	        GridRect  const rcGrid( m_pPixelCoordinates->Pixel2GridRect( pixRect ) );
            drawPOI( m_pCore->FindPOI( ) );
            drawIndividuals( rcGrid );
		 	m_pIndividualShape->SetClientWinHeight( pixRect.m_lBottom - pixRect.m_lTop );
            if ( m_pPixelCoordinates->GetFieldSize() >= 96 )
                drawText( rcGrid );
        }

        if ( m_pCore->SelectionIsNotEmpty() )
            m_pD3dBuffer->RenderTranspRect( m_pPixelCoordinates->Grid2PixelRect( m_pCore->GetSelection() ), D3DCOLOR_ARGB( 64, 0, 217, 255)  );  

        if ( pkgr.IsNotEmpty( ) )
            m_pD3dBuffer->RenderTranspRect( m_pPixelCoordinates->KGrid2PixelRect( pkgr ), D3DCOLOR_ARGB( 128, 255, 217, 0) );  

        m_pD3dBuffer->EndFrame( hwnd );  
    }
}

void DrawFrame::drawBackground( )
{
	float m_fPxSize = static_cast<float>( m_pPixelCoordinates->GetFieldSize( ) );

	Apply2Grid    // strip mode works only with full grid
	(          
    	[&](GridPoint const & gp)
		{
			int   const iValue  = m_pDspOptWindow->GetIntValue( Wrap2Grid(gp) );
			DWORD const dwColor = getBackgroundColor( iValue );
			m_pD3dBuffer->AddBackgroundPrimitive( m_pPixelCoordinates->Grid2PixelPosCenter( gp ), dwColor, m_fPxSize );
		},
		m_pD3dBuffer->GetStripMode() // if strip mode add borders to grid
	);

	m_pD3dBuffer->RenderBackground( );
}

void DrawFrame::drawPOI( GridPoint const & gpPoi )
{
    if ( gpPoi.IsNotNull( ) )
    {
        PixelPoint const ptCenter = m_pPixelCoordinates->Grid2PixelPosCenter( gpPoi );
		float      const fPixSize = static_cast<float>( m_pPixelCoordinates->GetFieldSize( ) );

        m_pD3dBuffer->AddIndividualPrimitive( ptCenter, CLR_WHITE, fPixSize * 0.50f );   // white frame for POI
        m_pD3dBuffer->AddIndividualPrimitive( ptCenter, CLR_BLACK, fPixSize * 0.45f );   // black frame for POI

        PlannedActivity const planPoi = m_pCore->GetPlan( );
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

void DrawFrame::drawIndividuals( GridRect const & rect )
{
    short const sFieldSize   = m_pPixelCoordinates->GetFieldSize();
    long  const lHalfSizeInd = ( sFieldSize <    8 ) ?                         1  :
                               ( sFieldSize <=  16 ) ? ((3 * sFieldSize) / 8 - 1) : 
                                                       ((3 * sFieldSize) / 8    );
	float const fHalfSizeInd = static_cast<float>( lHalfSizeInd );

    rect.Apply2Rect( [&](GridPoint const & gp) { setIndividualColor( gp, fHalfSizeInd ); } );

    m_pD3dBuffer->RenderIndividuals( ); 
}

void DrawFrame::drawText( GridRect const & rect )
{
    rect.Apply2Rect
	( 
		[&](GridPoint const & gp)
		{
            if ( m_pCore->IsAlive( gp ) )
            {
				m_pIndividualShape->Draw( gp );
            }
		}
	);
}

void DrawFrame::setIndividualColor( GridPoint const & gp, float const fHalfSize ) const
{
    tStrategyId const strat   = m_pCore->GetStrategyId( gp );
    short       const sEnergy = m_pCore->GetEnergy  ( gp );
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
