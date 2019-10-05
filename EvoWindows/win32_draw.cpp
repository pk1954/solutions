// win32_draw.cpp
//
// EvoWindows

#include "stdafx.h"
#include "EvolutionCore.h"
#include "GridDimensions.h"
#include "EvoPixelCoords.h"
#include "EvoReadBuffer.h"
#include "win32_util.h"
#include "win32_graphicsInterface.h"
#include "win32_displayOptions.h"
#include "win32_colorManager.h"
#include "win32_draw.h"

static COLORREF const CLR_BLACK  = RGB(   0,   0,   0 );
static COLORREF const CLR_GREY   = RGB( 128, 128, 128 );
static COLORREF const CLR_WHITE  = RGB( 255, 255, 255 );
static COLORREF const CLR_YELLOW = RGB( 255, 255,   0 );

DrawFrame::DrawFrame( )
 : 	m_hwnd             ( nullptr ),
	m_pReadBuffer      ( nullptr ),
	m_pEvoPixelCoords( nullptr ),
	m_pDspOptWindow    ( nullptr ),
	m_pColorManager    ( nullptr ),
	m_pGraphics        ( nullptr ), 
	m_pShapeHighlight  ( nullptr ), 
	m_clutBackground   ( ),
	m_gpHighlight      ( GP_NULL )
{ }

void DrawFrame::Start
( 
	HWND                const hwnd,
	EvoReadBuffer     * const pReadBuffer,
	EvoPixelCoords    * const pEvoPixelCoords, 
	GraphicsInterface * const pGraphics,
	DspOptWindow      * const pDspOptWindow,
	ColorManager      * const pColorManager
) 
{
	m_hwnd            = hwnd;
	m_pReadBuffer     = pReadBuffer;
	m_pEvoPixelCoords = pEvoPixelCoords;
	m_pDspOptWindow   = pDspOptWindow;
	m_pColorManager   = pColorManager;
	m_pGraphics       = pGraphics;
	m_pShapeHighlight = nullptr;
	m_gpHighlight     = GP_NULL;
	m_TextDisplay.Start( m_pGraphics, & m_wBuffer, m_pEvoPixelCoords );
	m_GridPointShape.Start( & m_TextDisplay );
	m_clutBackground.Allocate( MAX_BG_COLOR() );    // default is grey scale lookup table with entries 0 .. 255

	EvolutionCore const * pCore = m_pReadBuffer->LockReadBuffer( );
	assert( pCore != nullptr );
	m_GridPointShape.RefreshLayout( pCore );
	m_pReadBuffer->ReleaseReadBuffer( );
}

DrawFrame::~DrawFrame( ) 
{ 
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

void DrawFrame::ResizeDrawFrame( EvolutionCore const * const pCore )
{
	int   const MAX_TEXT_LINES = 10;
	PIXEL const pixFieldSize   = m_pEvoPixelCoords->GetFieldSize();
	PIXEL       pixFontSize    = pixFieldSize / MAX_TEXT_LINES;
	if ( pixFontSize < 9_PIXEL )
		pixFontSize = 9_PIXEL;
	if ( pixFontSize > 16_PIXEL )
		pixFontSize = 16_PIXEL;
    m_pGraphics->SetFontSize( pixFontSize );
	m_GridPointShape.RefreshLayout( pCore );
}

bool DrawFrame::SetHighlightPos( EvolutionCore const * const pCore, PixelPoint const ptCrsr )
{
	GridPoint const   gpLast     = m_gpHighlight;
	Shape     const * pShapeLast = m_pShapeHighlight;
	m_gpHighlight     = GridDimensions::Wrap2Grid( m_pEvoPixelCoords->Pixel2GridPos( ptCrsr ) );
	m_pShapeHighlight = m_GridPointShape.FindShape( pCore, m_gpHighlight, ptCrsr );
	return ( 
			  (m_pShapeHighlight != nullptr) && 
		      ( (pShapeLast != m_pShapeHighlight) || (gpLast != m_gpHighlight) )
		   );
	return false;
}

void DrawFrame::HighlightShape( Shape const * pShape, GridPoint const gp )
{
	PixelRect const & rect  = pShape->GetAbsoluteShapeRect( gp );
	COLORREF  const   color = m_pColorManager->GetColor( tColorObject::highlight );
	m_pGraphics->RenderTranspRect( rect, 128, color );  
}

void DrawFrame::DoPaint( EvolutionCore const * pCore )
{
	drawBackground( pCore );

	if ( m_pDspOptWindow->AreIndividualsVisible( ) )
	{
		GridRect const rcGrid( m_pEvoPixelCoords->Pixel2GridRect( Util::GetClPixelRect( m_hwnd ) ) );
		drawIndividuals( pCore, rcGrid );
		drawText( pCore, rcGrid );
		//if ( m_pShapeHighlight != nullptr )
		//{
		//	HighlightShape( m_pShapeHighlight, m_gpHighlight );
		//	GridPoint gpReferenced = m_pShapeHighlight->GetReferencedGridPoint( pCore, m_gpHighlight );
		//	if ( gpReferenced.IsNotNull() )
		//	{
		//		Shape const & shapeReferenced = m_GridPointShape.GetIndividualShape().GetLeftColumn().GetIdentifierShape();
		//		HighlightShape( & shapeReferenced, gpReferenced );
		//	}
		//}
	}
}

void DrawFrame::drawBackground( EvolutionCore const * const pCore )
{
	float m_fPxSize = static_cast<float>( m_pEvoPixelCoords->GetFieldSize( ).GetValue() );

	Apply2Grid    // strip mode works only with full grid
	(          
    	[&](GridPoint const gp)
		{
			CLUT_INDEX const index   { m_pDspOptWindow->GetIntValue( pCore, GridDimensions::Wrap2Grid(gp) ) };
			DWORD      const dwColor { getBackgroundColor( index ) };
			PixelPoint const pnt     { m_pEvoPixelCoords->Grid2PixelPosCenter( gp ) };
			m_pGraphics->AddBackGround( pnt, dwColor, m_fPxSize );
		},
		m_pGraphics->GetStripMode() // if strip mode add borders to grid
	);

	m_pGraphics->RenderBackground( );
}

void DrawFrame::addPrimitive( GridPoint const gp, COLORREF const color, float const fPixSizeHalf ) const
{
    if ( gp.IsNotNull( ) )
		m_pGraphics->AddIndividual( m_pEvoPixelCoords->Grid2PixelPosCenter( gp ), color, fPixSizeHalf );
}

void DrawFrame::drawIndividuals( EvolutionCore const * const pCore, GridRect const & rect )
{
	float const fHalfSizeInd = static_cast<float>( m_GridPointShape.GetIndShapeSize( ).GetValue() );
	float const fPixSize     = static_cast<float>( m_pEvoPixelCoords->GetFieldSize( ).GetValue() );

    Apply2Rect
	( 
		[&](GridPoint const gp) 
	    { 
			switch ( pCore->GetDisplayMode( gp ) )
			{
			case tDisplayMode::POI:
			{
				addPrimitive( gp, CLR_WHITE, fPixSize * 0.50f );   // white frame for POI
				addPrimitive( gp, CLR_BLACK, fPixSize * 0.45f );   // black frame for POI
				break;
			}
			case tDisplayMode::target:
			case tDisplayMode::partner:
			{
				addPrimitive( gp, CLR_GREY, fPixSize * 0.45f );   // mark target/partner
				break;
			}
			case tDisplayMode::neighbor:
			{
				addPrimitive( gp, CLR_YELLOW, fPixSize * 0.50f );   // mark neighbor
				break;
			}
			default:
				break;
			}

			if ( pCore->IsAlive( gp ) )
			{
				setIndividualColor( pCore, gp, fHalfSizeInd ); 
			}

			return false;
	    },
		rect
	);

    m_pGraphics->RenderForegroundObjects( ); 
}

void DrawFrame::drawText( EvolutionCore const * const pCore, GridRect const & rect )
{
    Apply2Rect
	( 
		[&](GridPoint const gp)
		{
			m_GridPointShape.Draw( pCore, gp );
			return false;
		},
		rect
	);
}

void DrawFrame::setIndividualColor( EvolutionCore const * const pCore, GridPoint const gp, float const fHalfSize ) const
{
	Strategy::Id const strat  { pCore->GetStrategyId( gp ) };
	ENERGY_UNITS const energy { pCore->GetEnergy    ( gp ) };

	assert( static_cast<int>( strat ) < Strategy::COUNT );
	assert( energy >= 0_ENERGY_UNITS );

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

void DrawFrame::AddContextMenuEntries( EvolutionCore const * const pCore, HMENU const hPopupMenu, POINT const pnt )
{
	PixelPoint    const pp     = Util::POINT2PixelPoint( pnt );
	GridPoint     const gp     = GridDimensions::Wrap2Grid( m_pEvoPixelCoords->Pixel2GridPos( pp ) );
	Shape const * const pShape = m_GridPointShape.FindShape( pCore, gp, pp );
	if ( pShape )
		pShape->AddContextMenuEntries( hPopupMenu );
}
