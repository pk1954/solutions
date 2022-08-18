// SigGenStaticData.h
//
// NNetModel

#pragma once

#include <iostream>

import Observable;
import BasicTypes;
import MoreTypes;
import VoltageType;
import fHertz;

using std::wcout;
using std::wostream;

class SigGenStaticData : public Observable
{
public:

	SigGenStaticData
	(
		BASE_PEAK<fHertz> f,
		BASE_PEAK<mV>     a,
		fMicroSecs        t
	)
	  :	m_freq(f),
		m_amplit(a),
		m_usPeak(t)
	{}

	SigGenStaticData()
      :	m_freq(10.0_fHertz, 50.0_fHertz),
		m_amplit(10._mV, 20._mV),
		m_usPeak(500000._MicroSecs)  // 1/2 sec
	{}

	bool operator==(SigGenStaticData const& rhs) const
	{
		return
		(m_freq   == rhs.m_freq  ) &&
		(m_amplit == rhs.m_amplit) &&
		(m_usPeak == rhs.m_usPeak);
	}

	SigGenStaticData & operator=(SigGenStaticData const& rhs)
	{
		m_freq   = rhs.m_freq;
		m_amplit = rhs.m_amplit;
		m_usPeak = rhs.m_usPeak;
		NotifyAll(false);
		return *this;
	}

	fMicroSecs                GetPeakTime () const { return m_usPeak; }
	BASE_PEAK<fHertz> const & GetFrequency() const { return m_freq;   }
	BASE_PEAK<mV>     const & GetAmplitude() const { return m_amplit; }

	void SetPeakTime(fMicroSecs const t) 
	{ 
		m_usPeak = t; 
		NotifyAll(false);
	}

	void SetFreq(BASE_PEAK<fHertz> const freq) 
	{ 
		m_freq = freq; 
		NotifyAll(false);
	}

	void SetFreqBase(fHertz const f) 
	{ 
		m_freq.SetBase(f); 
		NotifyAll(false);
	}

	void SetFreqPeak(fHertz const f) 
	{ 
		m_freq.SetPeak(f); 
		NotifyAll(false);
	}

	void SetAmpl(BASE_PEAK<mV> const amplit) 
	{ 
		m_amplit = amplit; 
		NotifyAll(false);
	}

	void SetAmplBase(mV const v) 
	{ 
		m_amplit.SetBase(v); 
		NotifyAll(false);
	}

	void SetAmplPeak(mV const v) 
	{ 
		m_amplit.SetPeak(v); 
		NotifyAll(false);
	}

	fMicroSecs CutoffTime() const 
	{ 
		return m_usPeak * CUT_OFF_FACTOR; 
	}

	bool InStimulusRange(fMicroSecs const t) const 
	{ 
		return t < CutoffTime(); 
	}

	mV GetStimulusAmplitude(fMicroSecs const uSecs)	const 
	{	
		return getStimulusValue<mV>(uSecs, m_amplit); 
	}

	fHertz GetStimulusFrequency(fMicroSecs const uSecs) const 
	{	
		return getStimulusValue<fHertz>(uSecs, m_freq  ); 
	}

	friend wostream & operator<< (wostream & out, SigGenStaticData const & data)
	{
		out << data.m_freq << data.m_amplit << L' ' << data.m_usPeak;
		return out;
	}

private:

	BASE_PEAK<fHertz> m_freq;
	BASE_PEAK<mV>     m_amplit;
	fMicroSecs        m_usPeak;

	inline static float const CUT_OFF_FACTOR { 10.0f };

	template <typename T>
	T getStimulusValue
	(
		fMicroSecs   const   t, 
		BASE_PEAK<T> const & par
	) const
	{
		T result { par.Base() };
		if (InStimulusRange(t))
		{
			float const fFactor   { t / m_usPeak };
			float const fStimulus { exp(1.f - fFactor) * fFactor };
			T     const boost     { par.Boost() };
			T     const stimulus  { boost * fStimulus };
			result += stimulus;
		}
		return result;
	}
};
