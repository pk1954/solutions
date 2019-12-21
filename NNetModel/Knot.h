// Knot.h
//
// NNetModel

#pragma once

#include <vector>
#include "Geometry.h"
#include "MoreTypes.h"
#include "tHighlightType.h"
#include "BaseKnot.h"
#include "Pipeline.h"

using std::vector;

class GraphicsInterface;
class PixelCoordsFp;
class NNetModel;

class Knot : public BaseKnot
{
public:
	Knot( NNetModel * pModel, MicroMeterPoint const center )
	  : BaseKnot( pModel, center, tShapeType::knot, PIPELINE_WIDTH / 2 )
	{}

	virtual ~Knot() {}

	static bool TypeFits( tShapeType const type )
	{
		return type == tShapeType::knot;
	}

	virtual void Prepare      ( );
	virtual void Step         ( ) { }
	virtual mV   GetNextOutput( ) const;
	virtual void DrawExterior ( PixelCoordsFp &, tHighlightType const ) const;
	virtual void DrawInterior ( PixelCoordsFp & ) const;

	virtual void DrawNeuronText( PixelCoordsFp & ) const {};
};
