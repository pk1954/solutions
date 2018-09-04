// pixelCoordinates.h : 
//

#pragma once

#include "pixelRect.h"
#include "gridPoint.h"
#include "gridRect.h"
#include "kGridPoint.h"
#include "SmoothMove.h"

class EvolutionModelData;

class PixelCoordinates
{
public:

    static short const MINIMUM_FIELD_SIZE  =    1;
    static short const DEFAULT_FIELD_SIZE  =    8;
    static short const MAXIMUM_FIELD_SIZE  = 1024;

    PixelCoordinates( short const, bool const );
    
	//////// transformations ////////

	PixelPoint Grid2PixelSize( GridPoint  const & ) const;

    GridPoint  Pixel2GridPos ( PixelPoint const & ) const;
	PixelPoint Grid2PixelPos ( GridPoint  const & ) const; 

    KGridPoint Pixel2KGridPos ( PixelPoint const & ) const;
    PixelPoint KGrid2PixelPos ( KGridPoint const & ) const; 

	KGridRect  Pixel2KGridRect( PixelRect const & ) const;
    PixelRect  KGrid2PixelRect( KGridRect const & ) const; 

    PixelRect  Grid2PixelRect ( GridRect  const & ) const;
    GridRect   Pixel2GridRect ( PixelRect const & ) const;

    PixelPoint Pixel2PixelSize( PixelPoint const &, PixelCoordinates const & ) const;
    PixelPoint Pixel2PixelPos ( PixelPoint const &, PixelCoordinates const & ) const;

	PixelPoint Grid2PixelPosCenter( GridPoint  const & ) const; 
	
	//////// queries ////////
	
	short      GetFieldSize( )   const { return m_sFieldSize; };
	PixelPoint GetPixelOffset( ) const { return m_pixOffset; }

	PixelRect  GetTextRect( GridPoint  const & ) const; 
	short      ComputeNewFieldSize( bool const ) const;

	//////// manipulation functions ////////

    bool  SetGridFieldSize( short, PixelPoint const );             // modifies field size
    bool  FitGridToRect( GridRect const &, PixelRectSize const );  // modifies field size

	bool  CenterPoi( PixelPoint const, GridPoint const );
    void  MoveGrid( PixelPoint const &);

private:
    bool       isValidFieldSize( long const ) const; 
    PixelPoint getCenterOffset( GridRect const &, PixelPoint const );

    PixelPoint m_pixOffset;
    short      m_sFieldSize;
    SmoothMove m_smoothMove;
    bool       m_bMoving;
	bool       m_bHexagon;
};
