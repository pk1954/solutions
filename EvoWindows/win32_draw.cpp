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
#include "win32_colorManager.h"
#include "win32_draw.h"

using namespace std;

DrawFrame::DrawFrame
( 
	HWND               const hwnd,
    EvolutionCore    * const pCore,
    PixelCoordinates * const pPixelCoordinates, 
    DspOptWindow     * const pDspOptWindow,
	ColorManager     * const pColorManager
) : 
    m_pCore( pCore ),
    m_pPixelCoordinates( pPixelCoordinates ),
    m_pDspOptWindow( pDspOptWindow ),
	m_pColorManager( pColorManager ),
    m_pD3dBuffer( nullptr ), 
    m_clutBackground( ),
	m_gridPointShape( nullptr )
{
	m_clutBackground.Allocate( MAX_BG_COLOR );    // default is grey scale lookup table with entries 0 .. 255
    m_pD3dBuffer      = new D3dBuffer( hwnd, GridPoint::GRID_AREA );
	m_pTextDisplay    = new TextDisplay( * m_pD3dBuffer, m_wBuffer, * m_pPixelCoordinates, * m_pCore );
	m_gridPointShape  = new GridPointShape( * m_pTextDisplay );
	m_pShapeHighlight = nullptr;
	m_gridPointShape->MinimalSize( );
}

DrawFrame::~DrawFrame( ) 
{ 
    delete m_pD3dBuffer;
    m_pD3dBuffer = nullptr;
};

void DrawFrame::CallStrategyColorDialog( HWND const hwndOwner, tStrategyId const strat )
{
	m_pColorManager->ColorDialog( hwndOwner, tColorObject::individual, strat );
}

void DrawFrame::CallSelectionColorDialog( HWND const hwndOwner )
{
	m_pColorManager->ColorDialog( hwndOwner, tColorObject::selection );
}

void DrawFrame::CallHighlightColorDialog( HWND const hwndOwner )
{
	m_pColorManager->ColorDialog( hwndOwner, tColorObject::highlight );
}

void DrawFrame::SetStripMode( tBoolOp const bOp ) 
{ 
    m_pD3dBuffer->SetStripMode( bOp ); 
};

void DrawFrame::ResizeDrawFrame( )
{
	short const sFieldSize     = m_pPixelCoordinates->GetFieldSize();
	int   const MAX_TEXT_LINES = 10;
	int         iFontSize      = sFieldSize / MAX_TEXT_LINES;
	if ( iFontSize < 9 )
		iFontSize = 9;
	if ( iFontSize > 16 )
		iFontSize = 16;
    m_pD3dBuffer->ResetFont( iFontSize );
	m_gridPointShape->RefreshLayout( );
}

bool DrawFrame::SetHighlightPos( PixelPoint const ptCrsr )
{
	GridPoint const   gpLast     = m_gpHighlight;
	Shape     const * pShapeLast = m_pShapeHighlight;
	m_gpHighlight = Wrap2Grid( m_pPixelCoordinates->Pixel2GridPos( ptCrsr ) );
	assert( m_gpHighlight.IsInGrid() );
	PixelPoint const ppGridpointOffset =  m_pPixelCoordinates->Grid2PixelPos( m_gpHighlight );
	m_pShapeHighlight = m_gridPointShape->FindShape( ptCrsr - ppGridpointOffset, m_gpHighlight );
	return ( (gpLast != m_gpHighlight) || (pShapeLast != m_pShapeHighlight) );
}

void DrawFrame::HighlightShape( Shape const * pShape, GridPoint const gp )
{
	PixelRect const & rect  = pShape->GetAbsoluteShapeRect( gp );
	COLORREF  const   color = m_pColorManager->GetColor( tColorObject::highlight );
	m_pD3dBuffer->RenderTranspRect( rect, 128, color );  
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
					HighlightShape( m_pShapeHighlight, m_gpHighlight );
					GridPoint gpReferenced = m_pShapeHighlight->GetReferencedGridPoint( m_gpHighlight );
					if ( gpReferenced != GridPoint::GP_NULL )
					{
						Shape const & shapeReferenced = m_gridPointShape->GetIndividualShape().GetLeftColumn().GetIdentifierShape();
						HighlightShape( & shapeReferenced, gpReferenced );
					}
				}
			}

			if ( m_pCore->SelectionIsNotEmpty() )
			{
				COLORREF const color = m_pColorManager->GetColor( tColorObject::selection );
				m_pD3dBuffer->RenderTranspRect( m_pPixelCoordinates->Grid2PixelRect( m_pCore->GetSelection() ), 64, color );  
			}

			if ( pkgr.IsNotEmpty( ) )
			{
				COLORREF const color = m_pColorManager->GetColor( tColorObject::selection );
				m_pD3dBuffer->RenderTranspRect( m_pPixelCoordinates->KGrid2PixelRect( pkgr ), 128, color );  
			}

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

void DrawFrame::addPrimitive( GridPoint const gp, COLORREF const color, float const fPixSize ) const
{
    if ( gp.IsNotNull( ) )
		m_pD3dBuffer->AddIndividualPrimitive( m_pPixelCoordinates->Grid2PixelPosCenter( gp ), color, fPixSize );
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
	float const fHalfSizeInd = static_cast<float>( m_gridPointShape->GetIndShapeSize( ) );

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
				PixelPoint const ppGridpointOffset = m_pPixelCoordinates->Grid2PixelPos( gp );
				m_gridPointShape->Draw( gp, ppGridpointOffset );
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
	COLORREF color = m_pColorManager->GetColor( tColorObject::individual, strat, uiIndex );
    addPrimitive( gp, color, fHalfSize );
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

    return m_clutBackground.GetColor( uiIndex );
}

void DrawFrame::AddContextMenuEntries( HMENU const hPopupMenu, POINT const pnt )
{
	PixelPoint    const pp     = Util::POINT2PixelPoint( pnt );
	GridPoint     const gp     = Wrap2Grid( m_pPixelCoordinates->Pixel2GridPos( pp ) );
	Shape const * const pShape = m_gridPointShape->FindShape( pp, gp );
	if ( pShape )
		pShape->AddContextMenuEntries( hPopupMenu );
}
