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

	virtual void Step( )
	{
	}

	virtual mV GetNextOutput( ) const
	{
		assert( m_mVinputBuffer <= PEAK_VOLTAGE );
		return m_mVinputBuffer;
	}

	virtual void DrawExterior( ) const;
	virtual void DrawInterior( ) const;

private:

};
