// NNetParameters.h
//
// NNetModel

#pragma once

#include <chrono>
#include "Observable.h"
#include "MoreTypes.h"
#include "ParameterType.h"

using namespace std::chrono;

class Param : public Observable
{
public:
	bool operator==(Param const & rhs) const;

	float const GetParameterValue(ParamType::Value const) const;
	void        SetParameterValue(ParamType::Value const, float const);

	fHertz      const StimulusMaxFreq() const { return m_stimulusMaxFreq; }
	fMicroSecs  const StimulusMaxTime() const { return m_stimulusMaxTime; }
	fHertz      const BaseFrequency  () const { return m_baseFrequency;   }
	mV          const Threshold      () const { return m_threshold; 	  }
	mV          const PeakVoltage    () const { return m_peakVoltage;     }
	fMicroSecs  const PulseWidth     () const { return m_pulseWidth;      }
	fMicroSecs  const RefractPeriod  () const { return m_refractPeriod;   }
	meterPerSec const PulseSpeed     () const { return m_pulseSpeed;      }
	fMicroSecs  const TimeResolution () const { return m_usResolution;    }

private:
	fHertz      m_stimulusMaxFreq { 50.0_fHertz       };
	fMicroSecs  m_stimulusMaxTime { 500000._MicroSecs };  // 1/2 sec
	fHertz      m_baseFrequency   { 10.0_fHertz       };
	meterPerSec m_pulseSpeed      { 120.0_meterPerSec };
	fMicroSecs  m_pulseWidth      { 2000._MicroSecs   };   
	mV          m_threshold       { 20._mV            };
	mV          m_peakVoltage     { 10._mV            };   
	fMicroSecs  m_refractPeriod   { 500._MicroSecs    };
	fMicroSecs  m_usResolution    { 100._MicroSecs    };
};

// geometry
inline static mV            const BASE_POTENTIAL  { 0.0_mV };
inline static float         const NEURON_INTERIOR { 0.8f };                // Neurons of all kinds have 80% interior, 20 % border 
inline static float         const PIPE_INTERIOR   { 0.6f };                // Pipes have 60% interior, 40 % border 
inline static MicroMeter    const NEURON_RADIUS   { 50.0_MicroMeter };    
inline static MicroMeter    const PIPE_WIDTH      { 20.0_MicroMeter };
inline static MicroMeterPnt const STD_OFFSET      { MicroMeterPnt(0._MicroMeter, NEURON_RADIUS * 2.0) };

static unsigned int CalcNrOfSteps(MicroMeter const um)
{
	static MicroMeter const umPerStep { NEURON_RADIUS / 5.0f };
	return Cast2UnsignedInt(um / umPerStep);
}

static unsigned int CalcNrOfSteps(Radian const rad)
{
	static Radian const radPerStep { Degrees2Radian(6.0_Degrees) };
	return Cast2UnsignedInt(rad / radPerStep);
}
