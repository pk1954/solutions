// Knot.h
//
// NNetModel

#pragma once

#include "Geometry.h"
#include "MoreTypes.h"
#include "tHighlightType.h"
#include "BaseKnot.h"
#include "Pipe.h"

class DrawContext;
class PixelCoordsFp;
class NNetModel;

using std::unique_ptr;
using std::make_unique;

class Knot : public BaseKnot
{
public:
	Knot( MicroMeterPoint const center )
		: BaseKnot( center, ShapeType::Value::knot, PIPE_WIDTH / 2 )
	{ }

	virtual ~Knot() {}

	void CheckShape( ) const;

	static  unsigned long GetCounter  ( ) { return m_counter; }
	static  void          ResetCounter( ) { m_counter = 0L; }

	virtual void IncCounter( ) { ++ m_counter; }
	virtual void DecCounter( ) { -- m_counter; }

	static bool TypeFits( ShapeType const type ) { return type.IsKnotType( ); }

	mV const GetNextOutput( ) const { return m_mVinputBuffer; }

	virtual bool const CompStep ( ) { return false; }

	virtual void DrawExterior( DrawContext const &, tHighlightType const ) const;
	virtual void DrawInterior( DrawContext const &, tHighlightType const ) const;
	virtual void Recalc      ( ) { };

	virtual void DrawNeuronText( PixelCoordsFp const & ) const {};

private: 
	inline static unsigned long m_counter { 0L };
};
