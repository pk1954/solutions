// NNetParameters.h
//
// NNetModel

#pragma once

#include <chrono>
#include "observable.h"
#include "MoreTypes.h"
#include "ParameterType.h"

class Param : public Observable
{
public:
	bool operator==(Param const & rhs) const;

	float       GetParameterValue(ParamType::Value const) const;
	void        SetParameterValue(ParamType::Value const, float const);

	BASE_PEAK<fHertz> const & InputFreq() const { return m_inputFreq; }
	BASE_PEAK<mV>     const & InputVolt() const { return m_inputVolt; }

	fMicroSecs  InputPeakTime () const { return m_inputPeakTime;  }
	mV          NeuronPeakVolt() const { return m_neuronPeakVolt; }
	mV          Threshold     () const { return m_threshold; 	  }
	fMicroSecs  PulseWidth    () const { return m_pulseWidth;     }
	fMicroSecs  RefractPeriod () const { return m_refractPeriod;  }
	meterPerSec PulseSpeed    () const { return m_pulseSpeed;     }
	fMicroSecs  TimeResolution() const { return m_usResolution;   }
	fMicroSecs  FilterSize    () const { return m_usFilterSize;   }

private:
	BASE_PEAK<fHertz> m_inputFreq     { 10.0_fHertz, 50.0_fHertz };
	BASE_PEAK<mV>     m_inputVolt     { 10._mV, 20._mV };   
	fMicroSecs        m_inputPeakTime { 500000._MicroSecs };  // 1/2 sec

	mV          m_neuronPeakVolt { 20._mV            };
	mV          m_threshold      { 20._mV            };
	meterPerSec m_pulseSpeed     { 120.0_meterPerSec };
	fMicroSecs  m_pulseWidth     { 2000._MicroSecs   };   
	fMicroSecs  m_refractPeriod  { 500._MicroSecs    };
	fMicroSecs  m_usResolution   { 100._MicroSecs    };
	fMicroSecs  m_usFilterSize   { m_usResolution    };
};

// geometry
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
