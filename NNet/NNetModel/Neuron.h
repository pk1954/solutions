// Neuron.h
//
// NNetModel

#pragma once

#include <chrono>
#include "MoreTypes.h"
#include "NNetParameters.h"
#include "tHighlightType.h"
#include "Pipe.h"
#include "BaseKnot.h"

class Neuron : public BaseKnot
{
public:
	Neuron( MicroMeterPoint const, ShapeType const = ShapeType::Value::neuron );
	virtual ~Neuron() {}

	static bool TypeFits( ShapeType const type ) { return type.IsAnyNeuronType( );	}

	bool      const HasAxon                 ( ) const { return m_outgoing.size() > 0;	}
	ShapeId   const GetAxonId               ( ) const { return HasAxon() ? m_outgoing[0]->GetId() : NO_SHAPE; }
	bool      const HasTriggerSound         ( ) const { return m_bTriggerSoundOn; }
	Hertz     const GetTriggerSoundFrequency( ) const { return m_triggerSoundFrequency; }
	MilliSecs const GetTriggerSoundDuration ( ) const {	return m_triggerSoundDuration; }

	void SetTriggerSoundFrequency( Hertz     const freq ) {	m_triggerSoundFrequency = freq;	}
	void SetTriggerSoundDuration ( MilliSecs const msec ) { m_triggerSoundDuration  = msec; }

	void SetTriggerSoundOn( bool const );

	fMicroSecs PulseWidth   ( ) const;
	fMicroSecs RefractPeriod( ) const;
	mV         Threshold    ( ) const;
	mV         PeakVoltage  ( ) const;

	virtual void DrawExterior( D2D_driver const &, PixelCoordsFp const &, tHighlightType const  = tHighlightType::normal ) const;
	virtual void DrawInterior( D2D_driver const &, PixelCoordsFp const & ) const;
	virtual void Recalc( );
	virtual void Clear( );
	virtual void Step( );
	virtual mV   GetNextOutput( ) const;

	virtual void DrawNeuronText( D2D_driver const &, PixelCoordsFp const & ) const;

protected:
	fMicroSecs m_timeSinceLastPulse { 0._MicroSecs };

	mV waveFunction( fMicroSecs const ) const;

	void drawExterior( D2D_driver const &, PixelCoordsFp const &, tHighlightType const ) const;

	void const DisplayText( D2D_driver const &, PixelRect const &, wstring const ) const;

private:
	mutable bool m_bTriggered { false };

	float m_factorW; // Parameter of wave function
	float m_factorU; // Parameter of wave function

	bool      m_bTriggerSoundOn       { false };
	Hertz     m_triggerSoundFrequency { 0_Hertz };   
	MilliSecs m_triggerSoundDuration  { 0_MilliSecs };

	PTP_WORK  m_pTpWork { nullptr };

	MicroMeterPoint getAxonHillockPos( PixelCoordsFp const & ) const;
};

Neuron const * Cast2Neuron( Shape const * );
Neuron       * Cast2Neuron( Shape       * );
