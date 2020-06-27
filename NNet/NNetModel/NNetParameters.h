// NNetParameters.h
//
// NNetModel

#pragma once

#include <chrono>
#include "Observable.h"
#include "MoreTypes.h"
#include "tParameter.h"

using namespace std::chrono;

class Param : public Observable
{
public:
	fMicroSecs const GetTimeResolution( ) const 
	{ 
		return m_usResolution; 
	}

	float const GetParameterValue( tParameter const ) const;
	void        SetParameterValue( tParameter const, float const );

	//bool const IsInEmphasizeMode( ) const           { return m_bEmphasizeMode; }
	//void       SetEmphasizeMode( bool const bMode ) { m_bEmphasizeMode = bMode; } 

private:
	//bool        m_bEmphasizeMode  { false };

	mV          m_threshold    { 20._mV            };
	mV          m_peakVoltage  { 10._mV            };   
	fMicroSecs  m_pulseWidth   { 2000._MicroSecs   };   
	fMicroSecs  m_refractPeriod{ 500._MicroSecs    };
	meterPerSec m_pulseSpeed   { 120.0_meterPerSec };
	fMicroSecs  m_usResolution { 100._MicroSecs    };
};

// dynamic model 
static fHertz          const STD_PULSE_FREQ  { 50.0_fHertz };         // Input neurons

// geometry
static mV              const BASE_POTENTIAL  { 0.0_mV };
static float           const NEURON_INTERIOR { 0.8f };                // Neurons of all kinds have 80% interior, 20 % border 
static float           const PIPE_INTERIOR   { 0.6f };                // Pipes have 60% interior, 40 % border 
static MicroMeter      const NEURON_RADIUS   { 50.0_MicroMeter };    
static MicroMeter      const PIPE_WIDTH      { 20.0_MicroMeter };
static MicroMeterPoint const STD_OFFSET      { MicroMeterPoint( 0._MicroMeter, NEURON_RADIUS * 2.0 ) };
