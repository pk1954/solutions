// ShapeId.h
//
// NNetModel

#pragma once

#include "SCRIPT.H"
#include "NamedType.h"

using ShapeId = NamedType< long, struct ShapeIdParam >;

inline ShapeId const NO_SHAPE( -1 );

static bool IsDefined( ShapeId const id ) 
{ 
	return id != NO_SHAPE; 
}

static bool IsUndefined( ShapeId const id ) 
{ 
	return id == NO_SHAPE; 
}

static ShapeId ScrReadShapeId( Script & script )
{
	ShapeId const id { static_cast<ShapeId>( script.ScrReadLong( ) ) };
	return id;
}
