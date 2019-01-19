// gridPoint.cpp :
//

#include "stdafx.h"
#include <limits.h>
#include <iostream>
#include <iomanip>
#include "gridPoint.h"

GridPoint const GridPoint::GP_NULL( GRID_COORD_NULL, GRID_COORD_NULL );
GridPoint const GridPoint::GRID_ORIGIN( GRID_X_MIN,  GRID_Y_MIN );
GridPoint const GridPoint::GRID_MAXIMUM( GRID_X_MAX, GRID_Y_MAX );
GridPoint const GridPoint::GRID_SIZE( GRID_WIDTH, GRID_HEIGHT );
GridPoint const GridPoint::GRID_CENTER( GRID_ORIGIN + GRID_SIZE / 2 );

std::wostream & operator << ( std::wostream & out, GridPoint const gp )
{
//lint -e747  Significant prototype coercion with setw
    out << L" " << std::setw(3) << gp.x << L" " << std::setw(3) << gp.y << L" ";
    return out;
}
