// SigGenData.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "observable.h"

using std::wcout;
using std::wostream;

class SigGenData : public Observable
{
public:

	SigGenData
	(
		BASE_PEAK<fHertz> f,
		BASE_PEAK<mV>     a,
		fMicroSecs        t
	)
	  :	m_freq(f),
		m_amplit(a),
		m_usPeak(t)
	{}

	SigGenData()
      :	m_freq(10.0_fHertz, 50.0_fHertz),
		m_amplit(10._mV, 20._mV),
		m_usPeak(500000._MicroSecs)  // 1/2 sec
	{}

	bool operator==(SigGenData const& rhs) const
	{
		return
		(m_freq   == rhs.m_freq  ) &&
		(m_amplit == rhs.m_amplit) &&
		(m_usPeak == rhs.m_usPeak);
	}

	SigGenData & operator=(SigGenData const& rhs)
	{
		m_freq   = rhs.m_freq;
		m_amplit = rhs.m_amplit;
		m_usPeak = rhs.m_usPeak;
		NotifyAll(false);
		return *this;
	}

	fMicroSecs                GetPeakTime() const { return m_usPeak; }
	BASE_PEAK<fHertz> const & GetFreq    () const { return m_freq;   }
	BASE_PEAK<mV>     const & GetAmplit  () const { return m_amplit; }

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

	mV GetAmplitude(fMicroSecs const uSecs)	const 
	{	
		return getActValue<mV>    (uSecs, m_amplit); 
	}

	fHertz GetFrequency(fMicroSecs const uSecs) const 
	{	
		return getActValue<fHertz>(uSecs, m_freq  ); 
	}

	friend wostream & operator<< (wostream & out, SigGenData const & data)
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
	T getActValue
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
