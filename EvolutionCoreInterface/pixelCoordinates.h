// pixelCoordinates.h : 
//

#pragma once

#include "pixelRect.h"
#include "gridPoint.h"
#include "gridRect.h"
#include "gridCircle.h"
#include "kGridPoint.h"
#include "EvolutionCore.h"
#include "SmoothMove.h"

class EvolutionModelData;

class PixelCoordinates
{
public:

    static short const MINIMUM_FIELD_SIZE  =    1;
    static short const DEFAULT_FIELD_SIZE  =    8;
    static short const MAXIMUM_FIELD_SIZE  = 1024;

    PixelCoordinates( short const, EvolutionCore *, EvolutionModelData * const, bool const );
    ~PixelCoordinates();

    bool IsPoiDefined ( ) const 
	{ 
		return m_pCore->IsPoiDefined( ); 
	};
	
	PixelPoint FindPoiCenter( ) const 
	{ 
		return Grid2PixelPosCenter( m_pCore->FindPOI( m_pModelWork ) ); 
	};
    
	short GetFieldSize ( ) const 
	{ 
		return m_sFieldSize; 
	};

    GridPoint Pixel2GridSize( PixelPoint const & pp ) const 
	{ 
		if ( m_bHexagon )
		{
			static double const FACTOR = 2 / sqrt( 3 );

			double const fDistX = FACTOR * (static_cast<double>( pp.x ) - 0.5) / static_cast<double>( m_sFieldSize );
			long   const lDistX = static_cast<long>( fDistX );

			return GridPoint( lDistX, pp.y / m_sFieldSize ); 
		}
		else
			return GridPoint( pp.x / m_sFieldSize, pp.y / m_sFieldSize ); 
	}

	PixelPoint Grid2PixelSize( GridPoint  const & gp ) const 
	{ 
		if ( m_bHexagon )
		{
			static double const FACTOR = sqrt( 3 ) / 2;

			double const fDistX = static_cast<double>( m_sFieldSize ) * FACTOR * static_cast<double>( gp.x );
			long   const lDistX = static_cast<long>( fDistX + 0.5 );

			return PixelPoint( lDistX, gp.y * m_sFieldSize ); 
		}
		else
		    return PixelPoint( gp.x * m_sFieldSize, gp.y * m_sFieldSize );
	}

    GridPoint  Pixel2GridPos ( PixelPoint const & pp ) const 
	{ 
		GridPoint gpResult;
		if ( m_bHexagon )  // does not work !!!!!!
		{
			double const dFieldSize = static_cast<double>( m_sFieldSize );
			double const dx = (double)pp.x / dFieldSize;
			double const dy = (double)pp.y / dFieldSize;
			double const temp = floor(dx + sqrt(3) * dy + 1);
			double const dq = floor((floor( 2 * dx + 1) + temp) / 3);
			double const dr = floor((temp + floor( -dx + sqrt(3) * dy + 1))/3);
			gpResult = GridPoint
			( 
				static_cast< long >( floor(dq + 0.5) ),
				static_cast< long >( floor(dr + 0.5) )
			);
			PixelPoint ppTest = Grid2PixelPos( gpResult );
			if (ppTest != pp)
			{
				int x = 76;
			}
		}
		else 
			gpResult = Pixel2GridSize( pp + m_pixOffset ); 

		return gpResult;
	}

    PixelPoint Grid2PixelPos ( GridPoint const & gp ) const 
	{ 
		PixelPoint ppRes = Grid2PixelSize( gp ) - m_pixOffset;
		if ( m_bHexagon )
		{
			if ( gp.IsOddCol( ) )
				ppRes.y -= m_sFieldSize / 2 ;
		}
		return ppRes;
	}

    GridCircle Pixel2GridCircle( PixelPoint const & pntCenter, short const sRadius ) const 
	{ 
		return GridCircle( Pixel2GridPos( pntCenter ), sRadius / m_sFieldSize ); 
	}

    KGridPoint Pixel2KGridPos( PixelPoint const & pp ) const 
	{ 
		return Pixel2KGridSize( pp + m_pixOffset, m_sFieldSize ); 
	}

    PixelPoint KGrid2PixelPos( KGridPoint const & kp ) const 
	{ 
		return KGrid2PixelSize( kp, m_sFieldSize ) - m_pixOffset; 
	}
    
	PixelPoint Grid2PixelPosCenter( GridPoint  const & gp ) const 
	{ 
		return Grid2PixelPos( gp ) + m_sFieldSize / 2; 
	}

    KGridRect  Pixel2KGridRect( PixelRect const & ) const;
    PixelRect  KGrid2PixelRect( KGridRect const & ) const; 

    PixelRect  Grid2PixelRect ( GridRect  const & ) const;
    GridRect   Pixel2GridRect ( PixelRect const & ) const;

    PixelPoint Pixel2PixelSize( PixelPoint const &, PixelCoordinates const & ) const;
    PixelPoint Pixel2PixelPos ( PixelPoint const &, PixelCoordinates const & ) const;

    bool CenterPoi( PixelPoint const );
    void SetPoi  ( PixelPoint const & );
    void MoveGrid( PixelPoint const &);
    bool Zoom( bool, PixelPoint const );
    bool SetFieldSize( short, PixelPoint const );
    bool FitToRect( GridRect const &, PixelRectSize const );

private:
    bool isValidFieldSize( long const lNewFieldSize ) const 
    { 
        return (MINIMUM_FIELD_SIZE <= lNewFieldSize) && (lNewFieldSize <= MAXIMUM_FIELD_SIZE); 
    };

    PixelPoint getCenterOffset( GridRect const &, PixelPoint const );

    PixelPoint           m_pixOffset;
    short                m_sFieldSize;
    SmoothMove           m_smoothMove;
    bool                 m_bMoving;
	bool                 m_bHexagon;
    EvolutionCore      * m_pCore;
    EvolutionModelData * m_pModelWork;
};
