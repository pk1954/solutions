// win32_frameBuffer.h : 
//

#pragma once

#include "pixelRect.h"
#include "gridPoint.h"
#include "gridRect.h"
#include "gridCircle.h"
#include "kGridPoint.h"
#include "EvolutionCore.h"
#include "SmoothMove.h"

class StatusBar;
class EvolutionModelData;

class FrameBuffer
{
public:

    static short const MINIMUM_FIELD_SIZE  =    1;
    static short const DEFAULT_FIELD_SIZE  =    8;
    static short const MAXIMUM_FIELD_SIZE  = 1024;

    FrameBuffer( HWND const, short const, StatusBar * const, EvolutionCore *, EvolutionModelData * const );
    ~FrameBuffer();

    BOOL CenterPoi( );
    void SetPoi  ( PixelPoint const & );
    void MoveGrid( PixelPoint const &);
    BOOL Zoom( BOOL );
    BOOL SetFieldSize( short );
    BOOL FitToRect( GridRect const & );

    BOOL       IsPoiDefined ( ) const { return m_pCore->IsPoiDefined( ); };
	PixelPoint FindPoiCenter( ) const { return Grid2PixelPosCenter( m_pCore->FindPOI( m_pModelWork ) ); };
    short      GetFieldSize ( ) const { return m_sFieldSize; };

    GridPoint  Pixel2GridSize( PixelPoint const & pp ) const { return GridPoint( pp.x / m_sFieldSize, pp.y / m_sFieldSize ); }
    PixelPoint Grid2PixelSize( GridPoint  const & gp ) const 
	{ 
		long   const lFS    = m_sFieldSize; 
		double const fDistX = static_cast<double>( lFS ) * sqrt( 3 ) / 2 * static_cast<double>( gp.x );
		long   const lDistX = static_cast<long>( fDistX + 0.5 );
		return PixelPoint( lDistX, gp.y * lFS ); 
	}

    GridPoint  Pixel2GridPos ( PixelPoint const & pp ) const 
	{ 
		return Pixel2GridSize( pp + m_pixOffset ); 
	}

    PixelPoint Grid2PixelPos ( GridPoint const & gp ) const 
	{ 
		PixelPoint ppRes = Grid2PixelSize( gp ) - m_pixOffset;
		if ( gp.IsOddCol( ) )
			ppRes.y -= m_sFieldSize / 2 ;
		return ppRes;
	}

    GridCircle Pixel2GridCircle( PixelPoint const & pntCenter, short const sRadius ) const { return GridCircle( Pixel2GridPos( pntCenter ), sRadius / m_sFieldSize ); }

    KGridRect  Pixel2KGridRect( PixelRect const & ) const;
    PixelRect  KGrid2PixelRect( KGridRect const & ) const; 

    PixelRect  Grid2PixelRect ( GridRect  const & ) const;
    GridRect   Pixel2GridRect ( PixelRect const & ) const;

    PixelPoint Pixel2PixelSize( PixelPoint const &, FrameBuffer const & ) const;
    PixelPoint Pixel2PixelPos ( PixelPoint const &, FrameBuffer const & ) const;

    KGridPoint Pixel2KGridPos     ( PixelPoint const & pp ) const { return Pixel2KGridSize( pp + m_pixOffset, m_sFieldSize ); }
    PixelPoint KGrid2PixelPos     ( KGridPoint const & kp ) const { return KGrid2PixelSize( kp, m_sFieldSize ) - m_pixOffset; }
    PixelPoint Grid2PixelPosCenter( GridPoint  const & gp ) const { return Grid2PixelPos( gp ) + m_sFieldSize / 2; }

private:
    BOOL isValidFieldSize( long const lNewFieldSize ) const 
    { 
        return (MINIMUM_FIELD_SIZE <= lNewFieldSize) && (lNewFieldSize <= MAXIMUM_FIELD_SIZE); 
    };

    PixelPoint getCenterOffset( GridRect const & );

    PixelPoint      m_pixOffset;
    short           m_sFieldSize;         // main and worker thread
    SmoothMove      m_smoothMove;
    BOOL            m_bMoving;
    HWND            m_hWnd;               // main thread only
    StatusBar     * m_pStatusBar; 
    EvolutionCore * m_pCore;
    EvolutionModelData     * m_pModelWork;
};
