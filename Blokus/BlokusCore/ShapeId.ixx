// BlokusCore.ixx
//
// ShapeId

export module BlokusCore:ShapeId;

import Types;

export using ShapeId = NamedType<signed char, struct ShapeId_Parameter>;

export int const UndefinedShapeId { -1 };
