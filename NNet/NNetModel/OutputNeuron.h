// OutputNeuron.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "ConnectionNeuron.h"

class OutputNeuron : public ConnNeuron
{
public:

	OutputNeuron( MicroMeterPoint const & );
	OutputNeuron( Neuron const & );

	virtual ~OutputNeuron() {};

	virtual void CheckShape() const;

	virtual bool operator==( Shape const & ) const override;

	static bool      const TypeFits( ShapeType const type ) { return type.IsOutputNeuronType(); }
	static ShapeType const GetShapeType() { return ShapeType::Value::outputNeuron; }

	virtual void DrawExterior( DrawContext const &, tHighlight const ) const;
	virtual void DrawInterior( DrawContext const &, tHighlight const ) const;

	virtual bool const IsPointInShape( MicroMeterPoint const & ) const;

private:

	void drawPlug
	( 
		DrawContext  const &, 
		float        const, 
		float        const, 
		D2D1::ColorF const
	) const;
};	
