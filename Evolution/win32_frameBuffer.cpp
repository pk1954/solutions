// win32_framebuffer.cpp
//

#include "stdafx.h"
#include "gridPoint.h"
#include "pixelPoint.h"
#include "EvolutionModelData.h"
#include "EvolutionCore.h"
#include "win32_util.h"
#include "win32_status.h"
#include "win32_frameBuffer.h"

FrameBuffer::FrameBuffer
( 
    HWND                 const hWnd, 
    short                const fs, 
    StatusBar          * const pStatusBar,
    EvolutionCore      *       pCore,
    EvolutionModelData * const pModel
)
  : m_pixOffset ( ),
    m_sFieldSize( fs ),
    m_smoothMove(  ),
    m_hWnd      ( hWnd ), 
    m_pStatusBar( pStatusBar ),
    m_pCore     ( pCore ),
    m_pModelWork( pModel )
{ }

FrameBuffer::~FrameBuffer()
{
    m_hWnd       = nullptr;
    m_pStatusBar = nullptr;
    m_pCore      = nullptr;
};

void FrameBuffer::SetPoi( PixelPoint const & pt )
{
    GridPoint const gpPoiNew = Pixel2GridPos( pt );
    if ( gpPoiNew.IsInGrid( ) )
    {
        IndId const idPoiNew = m_pModelWork->GetId( gpPoiNew );
        if ( idPoiNew.IsDefined( ) )
        {    
            if ( m_pCore->IsPoiId( idPoiNew ) )
                m_pCore->ClearPoi( );           // same POI. deactivate POI
            else
                m_pCore->SetPoi( m_pModelWork, gpPoiNew );
        }
    }
}

void FrameBuffer::MoveGrid( PixelPoint const & pntDelta )
{
    m_pixOffset -= pntDelta;
}

PixelPoint FrameBuffer::getCenterOffset(GridRect const & gridRect)  // Move gridRect to center of window
{
	assert(gridRect.GetStartPoint().IsInGrid());
	GridPoint  const gpStart(gridRect.GetStartPoint());
	GridPoint  const gpSum(gpStart + gpStart + gridRect.GetSize());
	PixelPoint const pixPnt(Grid2PixelSize(gpSum));
	PixelPoint const pixCenter(Util::GetClRectCenter(m_hWnd));
	PixelPoint const pixOffset(pixPnt / 2 - pixCenter);
	return pixOffset;
}

BOOL FrameBuffer::CenterPoi( ) // returns TRUE, if POI was already centered, or if no POI defined
{
    GridPoint const gpPoi = m_pCore->FindPOI( m_pModelWork );
    if ( gpPoi.IsNull( ) )
        return TRUE;

    PixelPoint pixCenterOffset = getCenterOffset( GridRect( gpPoi, gpPoi + 1 ) );
    BOOL       bCentered       = ( m_pixOffset == pixCenterOffset );

    if ( ! bCentered )
        m_pixOffset = m_smoothMove.Step( m_pixOffset, pixCenterOffset );

    return bCentered;
}

BOOL FrameBuffer::FitToRect( GridRect const & gridRect )
{
    PixelRectSize const pntPix = Util::GetClRectSize( m_hWnd );
    GridPoint gp( pntPix.GetWidth(), pntPix.GetHeight() );
    gp /= gridRect.GetSize() + 1;

    short const sNewFieldSize = min( gp.x, gp.y );
    
    if ( !isValidFieldSize( sNewFieldSize ) )
        return FALSE;

    m_sFieldSize = sNewFieldSize;
    m_pixOffset  = getCenterOffset( gridRect );

    if ( m_pStatusBar != nullptr )
        m_pStatusBar->SetSizeTrackBar( m_sFieldSize );

    return TRUE;
}

BOOL FrameBuffer::SetFieldSize( short const sNewFieldSize )
{
    if ( !isValidFieldSize( sNewFieldSize ) )
        return FALSE;
 
    PixelPoint const pntCenter = m_pCore->IsPoiDefined( ) 
                                ? Grid2PixelPosCenter( m_pCore->FindPOI( m_pModelWork ) )
                                : Util::GetClRectCenter( m_hWnd );

    m_pixOffset  = ((m_pixOffset + pntCenter) * sNewFieldSize) / m_sFieldSize - pntCenter;
    m_sFieldSize = sNewFieldSize;

    if ( m_pStatusBar != nullptr )
        m_pStatusBar->SetSizeTrackBar( m_sFieldSize );

    return TRUE;
}

BOOL FrameBuffer::Zoom( BOOL const bZoomIn )
{
    short sNewFieldSize = m_sFieldSize;
    if ( bZoomIn )
    {
        short const sDelta = ( m_sFieldSize < 16 ) ? 1 : (( m_sFieldSize < 64 ) ? 16 : 32);
        sNewFieldSize += sDelta;
    }
    else
    {
        short const sDelta = ( m_sFieldSize <= 16 ) ? 1 : (( m_sFieldSize <= 64 ) ? 16 : 32);
        sNewFieldSize -= sDelta;
    }

    return SetFieldSize( sNewFieldSize );
}

PixelPoint FrameBuffer::Pixel2PixelSize( PixelPoint const & ptSizeIn, FrameBuffer const & fTarget ) const 
{
    static long const FACTOR = 1024; // to avoid zero when dividing small ptSizeIn by m_sFieldSize 

    return (((ptSizeIn * FACTOR) / m_sFieldSize) * fTarget.m_sFieldSize ) / FACTOR;
}

PixelPoint FrameBuffer::Pixel2PixelPos( PixelPoint const & ptPosIn, FrameBuffer const & fTarget ) const 
{
    return Pixel2PixelSize( ptPosIn + m_pixOffset, fTarget ) - fTarget.m_pixOffset;
}

KGridRect FrameBuffer::Pixel2KGridRect( PixelRect const & rect ) const 
{
    PixelPoint pntStart( rect.left,  rect.top    );
    PixelPoint pntEnd  ( rect.right, rect.bottom );

    return KGridRect 
    (
        Pixel2KGridPos ( pntStart ),
        Pixel2KGridSize( pntEnd - pntStart, m_sFieldSize )
    );
}

PixelRect FrameBuffer::KGrid2PixelRect( KGridRect const & kgrIn ) const 
{
    PixelPoint const ptPos  = KGrid2PixelPos ( kgrIn.GetPos()  );
    PixelPoint const ptSize = KGrid2PixelSize( kgrIn.GetSiz(), m_sFieldSize );
    return PixelRect( ptPos, ptPos + ptSize );
}

GridRect FrameBuffer::Pixel2GridRect( PixelRect const & rect ) const 
{
    return GridRect
    ( 
        Pixel2GridPos( PixelPoint( rect.left,  rect.top    ) ), 
        Pixel2GridPos( PixelPoint( rect.right, rect.bottom ) ) 
    );
}

PixelRect FrameBuffer::Grid2PixelRect( GridRect const & rcGrid ) const 
{
    return PixelRect
    ( 
        Grid2PixelPos( rcGrid.GetStartPoint( ) ),
        Grid2PixelPos( rcGrid.GetEndPoint  ( ) ) + (m_sFieldSize - 1) 
    );
}
