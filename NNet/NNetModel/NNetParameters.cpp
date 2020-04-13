// NNetParameters.cpp
//
// NNetModel

#include "stdafx.h"
#include "NNetParameters.h"

Param::~Param( )
{
	m_observable.UnregisterAllObservers();
}

float const Param::GetParameterValue( tParameter const param ) const
{
	switch ( param )
	{
		case tParameter::pulseSpeed:	 return m_pulseSpeed.GetValue();
		case tParameter::pulseWidth:	 return m_pulseWidth.GetValue();
		case tParameter::threshold:  	 return m_threshold.GetValue();
		case tParameter::peakVoltage:	 return m_peakVoltage.GetValue();
		case tParameter::refractPeriod:  return m_refractPeriod.GetValue();
		case tParameter::timeResolution: return m_usResolution.GetValue();
		case tParameter::signalLoss:     return 0.0f;  // not used, only for compaitibility reasons
		default: assert( false );
	}
	return 0.f;
}

void Param::SetParameterValue
( 
	tParameter const param, 
	float      const fNewValue 
)
{
	switch ( param )
	{
		case tParameter::pulseSpeed:	 m_pulseSpeed    = static_cast< meterPerSec >( fNewValue ); break;
		case tParameter::pulseWidth:	 m_pulseWidth    = static_cast< fMicroSecs  >( fNewValue ); break;
		case tParameter::threshold:		 m_threshold     = static_cast< mV >         ( fNewValue ); break;
		case tParameter::peakVoltage:	 m_peakVoltage   = static_cast< mV >         ( fNewValue ); break;
		case tParameter::refractPeriod:	 m_refractPeriod = static_cast< fMicroSecs  >( fNewValue ); break;
		case tParameter::timeResolution: m_usResolution  = static_cast< fMicroSecs  >( fNewValue ); break;
		case tParameter::signalLoss: /* not used, only for compaitibility reasons */                break;
		default: assert( false );
	}
	m_observable.NotifyAll( FALSE );
}
