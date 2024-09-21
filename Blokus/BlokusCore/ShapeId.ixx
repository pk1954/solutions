// BlokusCore.ixx
//
// ShapeId

export module BlokusCore:ShapeId;

import Types;

export using ShapeId = NamedType<int, struct ShapeId_Parameter>;

export int const UndefinedShapeId { -1 };
