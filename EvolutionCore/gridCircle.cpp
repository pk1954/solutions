// gridCircle.cpp :
//

#include "stdafx.h"
#include <iostream>
#include <algorithm>  // min/max templates
#include "grid_model.h"
#include "gridPoint.h"
#include "gridRect.h"
#include "gridCircle.h"

using namespace std;

void Apply2Cone
( 
    const function<void( GridPoint const &, short const)>& func, 
    GridCircle const & circle, 
    short      const   sMaxIntensity  // between 0 and 100 (percent value)
)
{
    long const lRadius    = static_cast<long>(circle.GetRadius( ));
    long const lRadSquare = lRadius * lRadius;

    Apply2Rect
	( 
		[&](GridPoint const & gp, short const s)
		{
            GridPoint  gpDist      = gp - circle.GetCenter( );
            long const lDistSquare = static_cast<long>(gpDist.x) * static_cast<long>(gpDist.x) + static_cast<long>(gpDist.y) * static_cast<long>(gpDist.y);
            if ( lDistSquare <= lRadSquare )
            { 
                short const sReduce = static_cast<short>(( sMaxIntensity * lDistSquare) / lRadSquare);  
                func( gp, sMaxIntensity - sReduce );
            }
		},
	    GridRect( circle.GetCenter( ), circle.GetRadius( ) )
	);
}

GridCircle GetInscribedCircle( GridRect const & rect)
{
    return GridCircle( rect.GetCenter( ), rect.GetMinExtension( ) / 2 );
}

wostream & operator << ( std::wostream & out, GridCircle const & circle )
{
    out << circle.GetCenter() << L' ' << circle.GetRadius();
    return out;
}
