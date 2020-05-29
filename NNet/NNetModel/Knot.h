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

class DrawContext;
class NNetModel;

class Knot : public BaseKnot
{
public:
	Knot( MicroMeterPoint const center )
	  : BaseKnot( center, ShapeType::Value::knot, PIPE_WIDTH / 2 )
	{
		++ m_counter;
	}

	virtual ~Knot() 
	{
		-- m_counter;
	}

	static unsigned long GetCounter( ) { return m_counter; }

	static bool TypeFits( ShapeType const type ) { return type.IsKnotType( ); }

	mV GetNextOutput( ) const { return m_mVinputBuffer; }

	virtual bool CompStep    ( ) { return false; }
	virtual void DrawExterior( DrawContext const &, tHighlightType const = tHighlightType::normal ) const;
	virtual void DrawInterior( DrawContext const & ) const;
	virtual void Recalc      ( ) { };

	virtual void DrawNeuronText( PixelCoordsFp const & ) const {};

private: 
	inline static unsigned long m_counter { 0L };
};
