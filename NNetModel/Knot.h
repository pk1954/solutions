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
	  : BaseKnot( tShapeType::knot, center, 50.0_MicroMeter )
	{
	}

	virtual void Prepare( )
	{
		m_mVinputBuffer = 0._mV;
		for ( auto pipe : m_incomming )
			m_mVinputBuffer += pipe->GetNextOutput();
	}

	virtual void Step( )
	{
	}

	virtual mV GetNextOutput( ) const
	{
		return m_mVinputBuffer;
	}

	virtual void Draw( GraphicsInterface &,	PixelCoordsFp const & ) const {};

private:

};