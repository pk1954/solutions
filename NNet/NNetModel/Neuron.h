// Neuron.h
//
// NNetModel

#pragma once

#include <chrono>
#include "MoreTypes.h"
#include "NNetParameters.h"
#include "tHighlightType.h"
#include "BaseKnot.h"

class Neuron : public BaseKnot
{
public:
	Neuron( MicroMeterPoint const, ShapeType const = ShapeType::Value::neuron );
	virtual ~Neuron() {}

	static bool TypeFits( ShapeType const type )
	{
		return type.IsNeuronType( );
	}

	bool const HasAxon( ) const
	{
		return m_outgoing.size() > 1;
	}

	bool& TriggerSoundOn( )
	{
		return m_bTriggerSoundOn;
	}

	Hertz& TriggerSoundFrequency( )
	{
		return m_triggerSoundFrequency;
	}

	MilliSecs& TriggerSoundDuration( )
	{
		return m_triggerSoundDuration;
	}

	fMicroSecs PulseWidth   ( ) const;
	fMicroSecs RefractPeriod( ) const;
	mV         Threshold    ( ) const;
	mV         PeakVoltage  ( ) const;

	virtual void Step( );
	virtual mV   GetNextOutput( ) const;

	virtual void DrawExterior( PixelCoordsFp &, tHighlightType const ) const;
	virtual void DrawInterior( PixelCoordsFp & );
	virtual void Recalc( );

protected:
	fMicroSecs m_timeSinceLastPulse;

	mV waveFunction( fMicroSecs const ) const;

	void drawExterior( PixelCoordsFp &, tHighlightType const ) const;
	void drawInterior( PixelCoordsFp & ) const;

private:
	float m_factorW; // Parameter of wave function
	float m_factorU; // Parameter of wave function

	bool      m_bTriggerSoundOn;
	Hertz     m_triggerSoundFrequency;   
	MilliSecs m_triggerSoundDuration;

	MicroMeterPoint getAxonHillockPos( PixelCoordsFp & ) const;
};

Neuron const * Cast2Neuron( Shape const * );
Neuron       * Cast2Neuron( Shape       * );
