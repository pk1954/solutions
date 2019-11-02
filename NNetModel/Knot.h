// Knot.h
//
// NNetModel

#pragma once

#include <vector>
#include "Geometry.h"
#include "MoreTypes.h"
#include "BaseKnot.h"
#include "Pipeline.h"

using std::vector;

class GraphicsInterface;
class PixelCoordsFp;

class Knot : public BaseKnot
{
public:
	Knot( MicroMeterPoint const center )
	  : BaseKnot( center, tShapeType::knot, PIPELINE_WIDTH )
	{
	}

	virtual void Prepare( );
	virtual void Step( ) { }
	virtual mV GetNextOutput( ) const;
	virtual void DrawExterior( ) const;
	virtual void DrawInterior( ) const;
};
