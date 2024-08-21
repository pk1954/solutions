// InfoShape.ixx
//
// ShapeLib

export module InfoShape;

import std;
import Shape;

class EvolutionCore;

class InfoShape : public Shape
{
public:
	virtual void FillBuffer(EvolutionCore const * const, GridPoint const);
};
