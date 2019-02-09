// pixelCoordinates.h : 
//

#pragma once

#include "pixelRect.h"
#include "gridPoint.h"
#include "gridRect.h"
#include "kGridRect.h"
#include "SmoothMove.h"

class EvolutionCore;

class PixelCoordinates
{
public:

    PixelCoordinates( short const, bool const );
    
	//////// transformations ////////

	PixelPoint Grid2PixelSize( GridPoint  const ) const;

    GridPoint  Pixel2GridPos ( PixelPoint const ) const;
	PixelPoint Grid2PixelPos ( GridPoint  const ) const; 

    KGridPoint Pixel2KGridPos ( PixelPoint const ) const;
    PixelPoint KGrid2PixelPos ( KGridPoint const ) const; 

	KGridRect  Pixel2KGridRect( PixelRect const & ) const;
    PixelRect  KGrid2PixelRect( KGridRect const & ) const; 

    PixelRect  Grid2PixelRect ( GridRect  const & ) const;
    GridRect   Pixel2GridRect ( PixelRect const & ) const;

    PixelPoint Pixel2PixelSize( PixelPoint const, PixelCoordinates const & ) const;
    PixelPoint Pixel2PixelPos ( PixelPoint const, PixelCoordinates const & ) const;

	PixelPoint Grid2PixelPosCenter( GridPoint const ) const; 
	PixelRect  GridPoint2PixelRect( GridPoint const ) const;

	//////// queries ////////
	
	PIXEL      GetFieldSize( )   const { return m_pixFieldSize; };
	PixelPoint GetPixelOffset( ) const { return m_pixOffset; }

	PIXEL      ComputeNewFieldSize( bool const ) const;  // does not modify field size
    PIXEL	   CalcMaximumFieldSize( GridRectSize const &, PixelRectSize const & ) const;

	//////// manipulation functions ////////

    bool  SetGridFieldSize( PIXEL const );                                   
	void  CenterGrid( GridPoint const, PixelRectSize const );

	bool  CenterPoi( PixelPoint const, GridPoint const );
    void  MoveGrid( PixelPoint const);

private:
    bool       isValidFieldSize( PIXEL const ) const; 
    PixelPoint calcCenterOffset( GridPoint const, PixelPoint const );

    PixelPoint m_pixOffset;
    PIXEL      m_pixFieldSize;
    SmoothMove m_smoothMove;
    bool       m_bMoving;
	bool       m_bHexagon;
};

PIXEL const MINIMUM_FIELD_SIZE = PIXEL(1_PIXEL);
PIXEL const DEFAULT_FIELD_SIZE = PIXEL(8_PIXEL);
PIXEL const MAXIMUM_FIELD_SIZE = PIXEL(1024_PIXEL);
