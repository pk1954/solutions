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

void GridCircle::Apply2Shape
( 
    GridPointFuncShort const & func, 
    short              const   sMaxIntensity  // between 0 and 100 (percent value)
) const 
{
    long const lRadius    = static_cast<long>(m_gpExtension.x / 2);
    long const lRadSquare = lRadius * lRadius;

    GridRect( m_gpCenter, m_gpExtension.x ).Apply2Rect
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
    out << circle.GetCenter() << L' ' << circle.GetSize();
    return out;
}
