// win32_draw.cpp
//

#include "stdafx.h"
#include "config.h"
#include "EvolutionCore.h"
#include "pixelCoordinates.h"
#include "plannedActivity.h"
#include "win32_util.h"
#include "d3d_buffer.h"
#include "win32_displayOptions.h"
#include "win32_gridPointShape.h"
#include "win32_draw.h"

using namespace std;

static COLORREF const CLR_DEFECT    = RGB(151, 171, 255);
static COLORREF const CLR_COOPERATE = RGB(127, 255,   0);
static COLORREF const CLR_TIT4TAT   = RGB(255,  50,  50);

DrawFrame::DrawFrame
( 
	HWND               const hwnd,
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
	m_gridPointShape( nullptr )
{
    UINT const uiClutSize = static_cast<UINT>(Config::GetConfigValue( Config::tId::stdCapacity ));
    for ( auto & strategy : m_aClutStrat )
        strategy.Allocate( uiClutSize );

    m_pD3dBuffer = new D3dBuffer( hwnd, GridPoint::GRID_AREA );
	m_clutBackground.Allocate( MAX_BG_COLOR );    // default is grey scale lookup table with entries 0 .. 255
    SetIndDimmMode( Config::GetConfigValueBoolOp(Config::tId::dimmMode ) );

    SetStrategyColor( tStrategyId::defect,    CLR_DEFECT    );
    SetStrategyColor( tStrategyId::cooperate, CLR_COOPERATE );
    SetStrategyColor( tStrategyId::tit4tat,   CLR_TIT4TAT   );

	m_pTextDisplay    = new TextDisplay( * m_pD3dBuffer, m_wBuffer, * m_pPixelCoordinates, * m_pCore );
	m_gridPointShape  = new GridPointShape( * m_pTextDisplay );
	m_pShapeHighlight = nullptr;
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
	m_pD3dBuffer->Resize( );
}

void DrawFrame::prepareGridPoint( GridPoint const gp )
{
	PixelRect const rect = m_pPixelCoordinates->GridPoint2PixelRect( gp );
	m_gridPointShape->SetShapeRect( rect );
	m_gridPointShape->PrepareShape( gp );
}

bool DrawFrame::SetHighlightPos( PixelPoint const pos )
{
	GridPoint const   gpLast     = m_gpHighlight;
	Shape     const * pShapeLast = m_pShapeHighlight;
	m_gpHighlight = Wrap2Grid( m_pPixelCoordinates->Pixel2GridPos( pos ) );
	prepareGridPoint( m_gpHighlight );
	m_pShapeHighlight = m_gridPointShape->FindShape( pos, m_gpHighlight );
	return ( (gpLast != m_gpHighlight) || (pShapeLast != m_pShapeHighlight) );
}

void DrawFrame::HighlightShape( Shape const * pShape )
{
	PixelRect const & rect = pShape->GetAbsoluteCoordinates( );
	m_pD3dBuffer->RenderTranspRect( rect, D3DCOLOR_ARGB( 128, 255, 217, 0) );  
}

void DrawFrame::DoPaint( HWND hwnd, KGridRect const & pkgr )
{
    if ( IsWindowVisible( hwnd ) )
    {
		if ( m_pD3dBuffer->StartFrame( ) )
		{
			drawBackground( );

			if ( m_pDspOptWindow->AreIndividualsVisible( ) )
			{
				PixelRect const pixRect( Util::GetClPixelRect( hwnd ) );
				GridRect  const rcGrid( m_pPixelCoordinates->Pixel2GridRect( pixRect ) );
				drawPOI( m_pCore->FindPOI( ) );
				drawIndividuals( rcGrid );
				drawText( rcGrid );
				if ( m_pShapeHighlight != nullptr )
				{
					prepareGridPoint( m_gpHighlight );
					HighlightShape( m_pShapeHighlight);
					GridPoint gpReferenced = m_pShapeHighlight->GetReferencedGridPoint( m_gpHighlight );
					if ( gpReferenced != GridPoint::GP_NULL )
					{
						prepareGridPoint( gpReferenced );
						Shape const & shapeReferenced = m_gridPointShape->GetIndividualShape().GetLeftColumn().GetIdentifierShape();
						HighlightShape( & shapeReferenced );
					}
				}
			}

			if ( m_pCore->SelectionIsNotEmpty() )
				m_pD3dBuffer->RenderTranspRect( m_pPixelCoordinates->Grid2PixelRect( m_pCore->GetSelection() ), D3DCOLOR_ARGB( 64, 0, 217, 255) );  

			if ( pkgr.IsNotEmpty( ) )
				m_pD3dBuffer->RenderTranspRect( m_pPixelCoordinates->KGrid2PixelRect( pkgr ), D3DCOLOR_ARGB( 128, 255, 217, 0) );  

			m_pD3dBuffer->EndFrame( );
		}
    }
}

void DrawFrame::drawBackground( )
{
	float m_fPxSize = static_cast<float>( m_pPixelCoordinates->GetFieldSize( ) );

	Apply2Grid    // strip mode works only with full grid
	(          
    	[&](GridPoint const gp)
		{
			int        const iValue  = m_pDspOptWindow->GetIntValue( Wrap2Grid(gp) );
			DWORD      const dwColor = getBackgroundColor( iValue );
			PixelPoint const pnt     = m_pPixelCoordinates->Grid2PixelPosCenter( gp );
			m_pD3dBuffer->AddBackgroundPrimitive( pnt, dwColor, m_fPxSize );
		},
		m_pD3dBuffer->GetStripMode() // if strip mode add borders to grid
	);

	m_pD3dBuffer->RenderBackground( );
}

void DrawFrame::addPrimitive( GridPoint const gp, DWORD const dwColor, float const fPixSize ) const
{
    if ( gp.IsNotNull( ) )
		m_pD3dBuffer->AddIndividualPrimitive( m_pPixelCoordinates->Grid2PixelPosCenter( gp ), dwColor, fPixSize );
}

void DrawFrame::drawPOI( GridPoint const gpPoi )
{
    if ( gpPoi.IsNotNull( ) )
    {
        PixelPoint const ptCenter = m_pPixelCoordinates->Grid2PixelPosCenter( gpPoi );
		float      const fPixSize = static_cast<float>( m_pPixelCoordinates->GetFieldSize( ) );

        addPrimitive( gpPoi, CLR_WHITE, fPixSize * 0.50f );   // white frame for POI
        addPrimitive( gpPoi, CLR_BLACK, fPixSize * 0.45f );   // black frame for POI

        PlannedActivity const planPoi = m_pCore->GetPlan( );
        if ( planPoi.IsValid( ) )
        {
            addPrimitive( planPoi.GetTarget( ),  CLR_GREY, fPixSize * 0.45f );   // mark target
            addPrimitive( planPoi.GetPartner( ), CLR_GREY, fPixSize * 0.45f );   // mark target
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

    rect.Apply2Rect
	( 
		[&](GridPoint const gp) 
	    { 
            if ( m_pCore->IsAlive( gp ) )
            {
				setIndividualColor( gp, fHalfSizeInd ); 
			}
	    } 
	);

    m_pD3dBuffer->RenderIndividuals( ); 
}

void DrawFrame::drawText( GridRect const & rect )
{
    rect.Apply2Rect
	( 
		[&](GridPoint const gp)
		{
            if ( m_pCore->IsAlive( gp ) )
            {
				prepareGridPoint( gp );
				m_gridPointShape->Draw( gp );
            }
		}
	);
}

void DrawFrame::setIndividualColor( GridPoint const gp, float const fHalfSize ) const
{
    tStrategyId const strat = m_pCore->GetStrategyId( gp );
    if ( static_cast<int>( strat ) >= NR_STRATEGIES )
        return;                   // can happen in case of race conditions between display thread and worker thread
    //lint -e571  suspicious cast
    UINT const uiIndex = static_cast<UINT>( m_pCore->GetEnergy( gp ) );
    //lint +e571
    CLUT const & clut = m_aClutStrat.at( static_cast<int>( strat ) );
    assert( uiIndex < clut.GetSize() ); 
    addPrimitive( gp, clut.Get( uiIndex ),fHalfSize );
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
