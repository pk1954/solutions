// InfoShape.ixx
//
// ShapeLib

export module ShapeLib:InfoShape;

import std;
import EvoCoreLib;
import :Shape;

class InfoShape : public Shape
{
public:
	virtual void FillBuffer(EvolutionCore const&, GridPoint const);
};
