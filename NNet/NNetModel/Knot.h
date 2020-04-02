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
	Knot( MicroMeterPoint const center )
	  : BaseKnot( center, ShapeType::Value::knot, PIPELINE_WIDTH / 2 )
	{}

	virtual ~Knot() {}

	static bool TypeFits( ShapeType const type )
	{
		return type.IsKnotType( );
	}

	virtual mV GetNextOutput( ) const 
	{ 
		return m_mVinputBuffer; 
	}

	virtual void Step         ( ) { }
	virtual void DrawExterior ( PixelCoordsFp &, tHighlightType const ) const;
	virtual void DrawInterior ( PixelCoordsFp & );
	virtual void Recalc( ) {};

	virtual void DrawNeuronText( PixelCoordsFp & ) const {};
};
