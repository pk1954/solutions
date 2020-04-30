// Knot.h
//
// NNetModel

#pragma once

#include <vector>
#include "Geometry.h"
#include "MoreTypes.h"
#include "tHighlightType.h"
#include "BaseKnot.h"
#include "Pipe.h"

using std::vector;

class GraphicsInterface;
class PixelCoordsFp;
class NNetModel;

class Knot : public BaseKnot
{
public:
	Knot( MicroMeterPoint const center )
	  : BaseKnot( center, ShapeType::Value::knot, PIPE_WIDTH / 2 )
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
	virtual void DrawExterior ( D2D_driver const *, PixelCoordsFp &, tHighlightType const ) const;
	virtual void DrawInterior ( D2D_driver const *, PixelCoordsFp & );
	virtual void Recalc( ) {};

	virtual void DrawNeuronText( PixelCoordsFp & ) const {};
};
