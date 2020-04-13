// ShapeId.h
//
// NNetModel

#pragma once

#include "NamedType.h"

class Shape;
class D2D_driver;
class PixelCoordsFp;
class NNetModel;

using ShapeId = NamedType< long, struct ShapeIdParam >;

ShapeId const NO_SHAPE( -1 );

static bool IsDefined( ShapeId const id ) 
{ 
	return id != NO_SHAPE; 
}

static bool IsUndefined( ShapeId const id ) 
{ 
	return id == NO_SHAPE; 
}
