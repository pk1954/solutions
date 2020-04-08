// Neuron.h
//
// NNetModel

#pragma once

#include <chrono>
#include "MoreTypes.h"
#include "NNetParameters.h"
#include "tHighlightType.h"
#include "BaseKnot.h"

class BeeperThread;

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

	bool HasTriggerSound( ) const
	{
		return m_bTriggerSoundOn;
	}

	Hertz GetTriggerSoundFrequency( ) const
	{
		return m_triggerSoundFrequency;
	}

	MilliSecs GetTriggerSoundDuration( ) const 
	{
		return m_triggerSoundDuration;
	}

	void SetTriggerSoundOn( bool const );

	void SetTriggerSoundFrequency( Hertz const freq ) 
	{
		m_triggerSoundFrequency = freq;
	}

	void SetTriggerSoundDuration( MilliSecs const msec ) 
	{
		m_triggerSoundDuration = msec;
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

	virtual void Clear( );

protected:
	fMicroSecs m_timeSinceLastPulse { 0._MicroSecs };

	mV waveFunction( fMicroSecs const ) const;

	void drawExterior( PixelCoordsFp &, tHighlightType const ) const;
	void drawInterior( PixelCoordsFp & ) const;

private:
	float m_factorW; // Parameter of wave function
	float m_factorU; // Parameter of wave function

	bool      m_bTriggerSoundOn       { false };
	Hertz     m_triggerSoundFrequency { 0_Hertz };   
	MilliSecs m_triggerSoundDuration  { 0_MilliSecs };

	PTP_WORK  m_pTpWork { nullptr };

	MicroMeterPoint getAxonHillockPos( PixelCoordsFp & ) const;
};

Neuron const * Cast2Neuron( Shape const * );
Neuron       * Cast2Neuron( Shape       * );
