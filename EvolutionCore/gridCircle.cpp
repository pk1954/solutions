// gridCircle.cpp :
//

#include "stdafx.h"
#include <iostream>
#include <algorithm>  // min/max templates
#include "util.h"
#include "grid_model.h"
#include "gridPoint.h"
#include "gridRect.h"
#include "gridCircle.h"

using namespace std;

GridCircle const GridCircle::GRID_CIRCLE_EMPTY( GridPoint::GP_NULL, 0 );

void GridCircle::Apply2Circle
( 
    GridPointFuncShort const & func, 
    short              const   sMaxIntensity  // between 0 and 100 (percent value)
) const 
{
    long const lRadius    = static_cast<long>(m_radius);
    long const lRadSquare = lRadius * lRadius;

    GridRect( m_gpCenter, m_radius ).Apply2Rect
	( 
		[&](GridPoint const & gp)
		{
            GridPoint const gpDist      = gp - m_gpCenter;
			long      const lx          = static_cast<long>(gpDist.x);
			long      const ly          = static_cast<long>(gpDist.y);
            long      const lDistSquare = lx * lx + ly * ly;
            if ( lDistSquare <= lRadSquare )
            { 
                short const sReduce = CastToShort(( sMaxIntensity * lDistSquare) / lRadSquare);  
                func( gp, sMaxIntensity - sReduce );
            }
		}
	);
}

wostream & operator << ( std::wostream & out, GridCircle const & circle )
{
    out << circle.GetCenter() << L' ' << circle.GetRadius();
    return out;
}
