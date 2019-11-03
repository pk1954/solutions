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

	virtual void Prepare      ( NNetModel const & );
	virtual void Step         ( NNetModel const & ) { }
	virtual mV   GetNextOutput( NNetModel const & ) const;
	virtual void DrawExterior ( NNetModel const &, PixelCoordsFp & ) const;
	virtual void DrawInterior ( NNetModel const &, PixelCoordsFp & ) const;
};
