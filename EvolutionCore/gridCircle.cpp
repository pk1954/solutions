// gridCircle.cpp :
//

#include "stdafx.h"
#include <iostream>
#include <algorithm>  // min/max templates
#include "grid_model.h"
#include "gridPoint.h"
#include "gridRect.h"
#include "gridCircle.h"

void Apply2Cone
( 
    const std::function<void( GridPoint const &, short const)>& func, 
    GridCircle                   const & circle, 
    short                        const   sMaxIntensity  // between 0 and 100 (percent value)
)
{
    GridRect rect( circle.GetCenter( ), circle.GetRadius( ) );
    GRID_COORD   const lLeft      = max( rect.GetLeft  (), GridRect::GRID_RECT_FULL.GetLeft  () );
    GRID_COORD   const lTop       = max( rect.GetTop   (), GridRect::GRID_RECT_FULL.GetTop   () );
    GRID_COORD   const lRight     = min( rect.GetRight (), GridRect::GRID_RECT_FULL.GetRight () );
    GRID_COORD   const lBottom    = min( rect.GetBottom(), GridRect::GRID_RECT_FULL.GetBottom() );
    long         const lRadius    = static_cast<long>(circle.GetRadius( ));
    long         const lRadSquare = lRadius * lRadius;

    GridPoint gp;
    for (gp.y = lTop; gp.y <= lBottom; ++gp.y)
        for (gp.x = lLeft; gp.x <= lRight; ++gp.x)
        {
            GridPoint  gpDist      = gp - circle.GetCenter( );
            long const lDistSquare = static_cast<long>(gpDist.x) * static_cast<long>(gpDist.x) + static_cast<long>(gpDist.y) * static_cast<long>(gpDist.y);
            if ( lDistSquare <= lRadSquare )
            { 
                short const sReduce = static_cast<short>(( sMaxIntensity * lDistSquare) / lRadSquare);  
                func( gp, sMaxIntensity - sReduce );
            }
        }
}

GridCircle GetInscribedCircle( GridRect const & rect)
{
    return GridCircle( rect.GetCenter( ), rect.GetMinExtension( ) / 2 );
}

std::wostream & operator << ( std::wostream & out, GridCircle const & circle )
{
    out << circle.GetCenter() << L' ' << circle.GetRadius();
    return out;
}
