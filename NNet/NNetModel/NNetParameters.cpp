// NNetParameters.cpp
//
// NNetModel

#include "stdafx.h"
#include "NNetParameters.h"

bool Param::operator==( Param const & rhs ) const
{
	return
		(m_threshold     == rhs.m_threshold    ) && 
		(m_peakVoltage   == rhs.m_peakVoltage  ) &&
		(m_pulseWidth    == rhs.m_pulseWidth   ) &&
		(m_refractPeriod == rhs.m_refractPeriod) &&
		(m_pulseSpeed    == rhs.m_pulseSpeed   ) &&
		(m_usResolution  == rhs.m_usResolution );
}

float const Param::GetParameterValue( ParamType::Value const param ) const
{
	switch ( param )
	{
		case ParamType::Value::pulseSpeed:	   return m_pulseSpeed.GetValue();
		case ParamType::Value::pulseWidth:	   return m_pulseWidth.GetValue();
		case ParamType::Value::threshold:  	   return m_threshold.GetValue();
		case ParamType::Value::peakVoltage:	   return m_peakVoltage.GetValue();
		case ParamType::Value::refractPeriod:  return m_refractPeriod.GetValue();
		case ParamType::Value::timeResolution: return m_usResolution.GetValue();
		case ParamType::Value::signalLoss:     return 0.0f;  // no longer used, only for backward compatibility
		default: assert( false );
	}
	return 0.f;
}

void Param::SetParameterValue
( 
	ParamType::Value const param, 
	float            const fNewValue 
)
{
	switch ( param )
	{
		case ParamType::Value::pulseSpeed:	   m_pulseSpeed    = static_cast< meterPerSec >(fNewValue); break;
		case ParamType::Value::pulseWidth:	   m_pulseWidth    = static_cast< fMicroSecs  >(fNewValue); break;
		case ParamType::Value::threshold:	   m_threshold     = static_cast< mV >         (fNewValue); break;
		case ParamType::Value::peakVoltage:	   m_peakVoltage   = static_cast< mV >         (fNewValue); break;
		case ParamType::Value::refractPeriod:  m_refractPeriod = static_cast< fMicroSecs  >(fNewValue); break;
		case ParamType::Value::timeResolution: m_usResolution  = static_cast< fMicroSecs  >(fNewValue); break;
		case ParamType::Value::signalLoss: break; // legacy
		default: assert( false );
	}

	NotifyAll( false );
}
