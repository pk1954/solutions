// NNetModel.h 
//
// ShapeErrorHandler.h

#pragma once

#include "ShapeId.h"

class ShapeErrorHandler
{
public:
	virtual void operator()( ShapeId const ) = 0;
};
