// win32_draw.cpp
//
// EvoWindows

#include "stdafx.h"
#include "EvolutionCore.h"
#include "GridDimensions.h"
#include "pixelCoordinates.h"
#include "plannedActivity.h"
#include "win32_util.h"
#include "win32_graphicsInterface.h"
#include "win32_displayOptions.h"
#include "win32_gridPointShape.h"
#include "win32_colorManager.h"
#include "win32_draw.h"

static COLORREF const CLR_BLACK = RGB(   0,   0,   0 );
static COLORREF const CLR_GREY  = RGB( 128, 128, 128 );
static COLORREF const CLR_WHITE = RGB( 255, 255, 255 );

DrawFrame::DrawFrame
( 
	HWND                const hwnd,
    EvolutionCore     * const pCore,
    PixelCoordinates  * const pPixelCoordinates, 
	GraphicsInterface * const pGraphics,
    DspOptWindow      * const pDspOptWindow,
	ColorManager      * const pColorManager
) : 
	m_hwnd( hwnd ),
    m_pCore( pCore ),
    m_pPixelCoordinates( pPixelCoordinates ),
    m_pDspOptWindow( pDspOptWindow ),
	m_pColorManager( pColorManager ),
    m_pGraphics( pGraphics ), 
    m_clutBackground( ),
	m_gridPointShape( nullptr ),
	m_gpHighlight( GridPoint::NULL_VAL() )
{
	m_clutBackground.Allocate( MAX_BG_COLOR() );    // default is grey scale lookup table with entries 0 .. 255
	m_pTextDisplay    = new TextDisplay( * m_pGraphics, m_wBuffer, * m_pPixelCoordinates, * m_pCore );
	m_gridPointShape  = new GridPointShape( * m_pTextDisplay );
	m_pShapeHighlight = nullptr;
	m_gridPointShape->RefreshLayout( );
}

DrawFrame::~DrawFrame( ) 
{ 
    delete m_pGraphics;
    m_pGraphics = nullptr;
};

void DrawFrame::CallStrategyColorDialog( HWND const hwndOwner, Strategy::Id const strat )
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
    m_pGraphics->SetStripMode( bOp ); 
};

void DrawFrame::ResizeDrawFrame( )
{
	int   const MAX_TEXT_LINES = 10;
	PIXEL const pixFieldSize   = m_pPixelCoordinates->GetFieldSize();
	PIXEL       pixFontSize    = pixFieldSize / MAX_TEXT_LINES;
	if ( pixFontSize < 9_PIXEL )
		pixFontSize = 9_PIXEL;
	if ( pixFontSize > 16_PIXEL )
		pixFontSize = 16_PIXEL;
    m_pGraphics->SetFontSize( pixFontSize );
	m_gridPointShape->RefreshLayout( );
}

bool DrawFrame::SetHighlightPos( PixelPoint const ptCrsr )
{
	GridPoint const   gpLast     = m_gpHighlight;
	Shape     const * pShapeLast = m_pShapeHighlight;
	m_gpHighlight = GridDimensions::Wrap2Grid( m_pPixelCoordinates->Pixel2GridPos( ptCrsr ) );
	assert( IsInGrid( m_gpHighlight ) );
	PixelPoint const ppGridpointOffset =  m_pPixelCoordinates->Grid2PixelPos( m_gpHighlight );
	m_pShapeHighlight = m_gridPointShape->FindShape( ptCrsr - ppGridpointOffset, m_gpHighlight );
	return ( (gpLast != m_gpHighlight) || (pShapeLast != m_pShapeHighlight) );
}

void DrawFrame::HighlightShape( Shape const * pShape, GridPoint const gp )
{
	PixelRect const & rect  = pShape->GetAbsoluteShapeRect( gp );
	COLORREF  const   color = m_pColorManager->GetColor( tColorObject::highlight );
	m_pGraphics->RenderTranspRect( rect, 128, color );  
}

void DrawFrame::DoPaint( )
{
	drawBackground( );

	if ( m_pDspOptWindow->AreIndividualsVisible( ) )
	{
		GridRect const rcGrid( m_pPixelCoordinates->Pixel2GridRect( Util::GetClPixelRect( m_hwnd ) ) );
		drawPOI( m_pCore->FindPOI( ) );
		drawIndividuals( rcGrid );
		drawText( rcGrid );
		if ( m_pShapeHighlight != nullptr )
		{
			HighlightShape( m_pShapeHighlight, m_gpHighlight );
			GridPoint gpReferenced = m_pShapeHighlight->GetReferencedGridPoint( m_gpHighlight );
			if ( gpReferenced.IsNotNull() )
			{
				Shape const & shapeReferenced = m_gridPointShape->GetIndividualShape().GetLeftColumn().GetIdentifierShape();
				HighlightShape( & shapeReferenced, gpReferenced );
			}
		}
	}
}

void DrawFrame::drawBackground( )
{
	float m_fPxSize = static_cast<float>( m_pPixelCoordinates->GetFieldSize( ).GetValue() );

	Apply2Grid    // strip mode works only with full grid
	(          
    	[&](GridPoint const gp)
		{
			CLUT_INDEX const index   { m_pDspOptWindow->GetIntValue( GridDimensions::Wrap2Grid(gp) ) };
			DWORD      const dwColor { getBackgroundColor( index ) };
			PixelPoint const pnt     { m_pPixelCoordinates->Grid2PixelPosCenter( gp ) };
			m_pGraphics->AddBackGround( pnt, dwColor, m_fPxSize );
		},
		m_pGraphics->GetStripMode() // if strip mode add borders to grid
	);

	m_pGraphics->RenderBackground( );
}

void DrawFrame::addPrimitive( GridPoint const gp, COLORREF const color, float const fPixSizeHalf ) const
{
    if ( gp.IsNotNull( ) )
		m_pGraphics->AddIndividual( m_pPixelCoordinates->Grid2PixelPosCenter( gp ), color, fPixSizeHalf );
}

void DrawFrame::drawPOI( GridPoint const gpPoi )
{
    if ( gpPoi.IsNotNull( ) )
    {
        PixelPoint const ptCenter = m_pPixelCoordinates->Grid2PixelPosCenter( gpPoi );
		float      const fPixSize = static_cast<float>( m_pPixelCoordinates->GetFieldSize( ).GetValue() );

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
	float const fHalfSizeInd = static_cast<float>( m_gridPointShape->GetIndShapeSize( ).GetValue() );

    Apply2Rect
	( 
		[&](GridPoint const gp) 
	    { 
            if ( m_pCore->IsAlive( gp ) )
            {
				setIndividualColor( gp, fHalfSizeInd ); 
			}
	    },
		rect
	);

    m_pGraphics->RenderIndividuals( ); 
}

void DrawFrame::drawText( GridRect const & rect )
{
    Apply2Rect
	( 
		[&](GridPoint const gp)
		{
            if ( m_pCore->IsAlive( gp ) )
            {
				PixelPoint const ppGridpointOffset = m_pPixelCoordinates->Grid2PixelPos( gp );
				m_gridPointShape->Draw( gp, ppGridpointOffset );
            }
		},
		rect
	);
}

void DrawFrame::setIndividualColor( GridPoint const gp, float const fHalfSize ) const
{
    Strategy::Id const strat  = m_pCore->GetStrategyId( gp );
    ENERGY_UNITS const energy = m_pCore->GetEnergy( gp );

	if ( static_cast<int>( strat ) >= Strategy::COUNT )  // can happen in case of
        return;                                          // race conditions between 
	if ( energy < 0_ENERGY_UNITS )                      // display thread and 
		return;                                          // worker thread

	CLUT_INDEX const index { CastToInt( energy.GetValue() ) };
	COLORREF   const color { m_pColorManager->GetColor( tColorObject::individual, strat, index ) };
    addPrimitive( gp, color, fHalfSize );
}

COLORREF DrawFrame::getBackgroundColor( CLUT_INDEX index ) const
{
    if ( index < CLUT_INDEX(0) )
        index = CLUT_INDEX(0);
    else if ( index > MAX_BG_COLOR() )
        index = MAX_BG_COLOR();
    return m_clutBackground.GetColor( index );
}

void DrawFrame::AddContextMenuEntries( HMENU const hPopupMenu, POINT const pnt )
{
	PixelPoint    const pp     = Util::POINT2PixelPoint( pnt );
	GridPoint     const gp     = GridDimensions::Wrap2Grid( m_pPixelCoordinates->Pixel2GridPos( pp ) );
	Shape const * const pShape = m_gridPointShape->FindShape( pp, gp );
	if ( pShape )
		pShape->AddContextMenuEntries( hPopupMenu );
}
