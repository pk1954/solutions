// gridCircle.h : 
//

#pragma once

#include <iostream>
#include "EvolutionTypes.h"
#include "gridShape.h"
#include "gridPoint.h"

class GridCircle : public Shape
{
public:
    GridCircle( GridPoint const & gpCenter, GRID_COORD const iSize ) 
		: Shape( gpCenter, iSize )
	{};

	virtual void Apply2Shape( GridPointFuncShort const &, short const = 0) const;
};

std::wostream & operator << ( std::wostream &, GridCircle const & );
