// OutputNeuron.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "ConnectionNeuron.h"

class OutputNeuron : public ConnectionNeuron
{
public:

	OutputNeuron( MicroMeterPoint const );

	virtual ~OutputNeuron() {};

	virtual void CheckShape() const;

	virtual bool operator==( Shape const & ) const override;

	static unsigned long GetCounter  () { return m_counter; }
	static   void        ResetCounter() { m_counter = 0L; }
	virtual  void        IncCounter  () { ++ m_counter; }
	virtual  void        DecCounter  () { -- m_counter; }

	static bool TypeFits( ShapeType const type ) { return type.IsOutputNeuronType(); }

	virtual void DrawExterior( DrawContext const &, tHighlightType const ) const;
	virtual void DrawInterior( DrawContext const &, tHighlightType const ) const;

private:
	void drawPlug
	( 
		DrawContext  const &, 
		float        const, 
		float        const, 
		D2D1::ColorF const
	) const;

	inline static unsigned long m_counter { 0L };
};	
