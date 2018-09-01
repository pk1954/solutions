// gridShape.h
//

#pragma once

#include "gridPoint.h"

class Shape
{
public:
	virtual ~Shape() {};

	virtual void Apply2Shape( GridPointFuncShort const &, short const = 0 ) const = 0;

	virtual void Reset( ) = 0;

    virtual bool const IsEmpty( )    const = 0;
    virtual bool const IsNotEmpty( ) const = 0;

	virtual GridPoint  const GetCenter( ) const = 0;
};
