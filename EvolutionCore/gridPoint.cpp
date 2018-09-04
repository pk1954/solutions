// gridPoint.cpp :
//

#include "stdafx.h"
#include <limits.h>
#include <iostream>
#include <iomanip>
#include "gridPoint.h"

GridPoint const GridPoint::GP_NULL( SHRT_MIN, SHRT_MIN );
GridPoint const GridPoint::GRID_ORIGIN( 0,  0 );
GridPoint const GridPoint::GRID_MAXIMUM( GridPoint::GRID_WIDTH - 1, GridPoint::GRID_HEIGHT - 1 );
GridPoint const GridPoint::GRID_SIZE( GridPoint::GRID_WIDTH, GridPoint::GRID_HEIGHT );
GridPoint const GridPoint::GRID_CENTER( GRID_ORIGIN + GRID_SIZE / 2 );

std::wostream & operator << ( std::wostream & out, GridPoint const & gp )
{
//lint -e747  Significant prototype coercion with setw
    out << L" " << std::setw(3) << gp.x << L" " << std::setw(3) << gp.y << L" ";
    return out;
}
