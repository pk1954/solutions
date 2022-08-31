// SigGenStaticData.ixx
//
// NNetModel

module;

#include <iostream>

export module SigGenStaticData;

import Observable;
import BasicTypes;
import MoreTypes;
import BasePeak;

using std::wcout;
using std::wostream;

export class SigGenStaticData : public Observable
{
public:

	SigGenStaticData
	(
		BasePeak<fHertz> f,
		BasePeak<mV>     a,
		fMicroSecs       t
	)
	  : m_freq(f),
		m_amplit(a),
		m_usPeak(t)
	{}

	SigGenStaticData()
	  : m_freq(10.0_fHertz, 50.0_fHertz),
		m_amplit(10._mV, 20._mV),
		m_usPeak(500000._MicroSecs)  // 1/2 sec
	{}

	bool operator==(SigGenStaticData const& rhs) const
	{
		return
			(m_freq   == rhs.m_freq)   &&
			(m_amplit == rhs.m_amplit) &&
			(m_usPeak == rhs.m_usPeak);
	}

	SigGenStaticData& operator=(SigGenStaticData const& rhs)
	{
		m_freq   = rhs.m_freq;
		m_amplit = rhs.m_amplit;
		m_usPeak = rhs.m_usPeak;
		NotifyAll(false);
		return *this;
	}

	fMicroSecs                GetPeakTime() const { return m_usPeak; }
	BasePeak<fHertz> const & GetFrequency() const { return m_freq; }
	BasePeak<mV>     const & GetAmplitude() const { return m_amplit; }

	void SetPeakTime(fMicroSecs const t)
	{
		m_usPeak = t;
		NotifyAll(false);
	}

	void SetFreq(BasePeak<fHertz> const freq)
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

	void SetAmpl(BasePeak<mV> const amplit)
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
		return getStimulusValue<fHertz>(uSecs, m_freq);
	}

	friend wostream& operator<< (wostream& out, SigGenStaticData const& data)
	{
		out << data.m_freq << data.m_amplit << L' ' << data.m_usPeak;
		return out;
	}

private:

	BasePeak<fHertz> m_freq;
	BasePeak<mV>     m_amplit;
	fMicroSecs       m_usPeak;

	inline static float const CUT_OFF_FACTOR{ 10.0f };

	template <typename T>
	T getStimulusValue
	(
		fMicroSecs  const   t,
		BasePeak<T> const & par
	) const
	{
		T result { par.Base() };
		if (InStimulusRange(t))
		{
			float const fFactor{ t / m_usPeak };
			float const fStimulus{ exp(1.f - fFactor) * fFactor };
			T     const boost{ par.Boost() };
			T     const stimulus{ boost * fStimulus };
			result += stimulus;
		}
		return result;
	}
};
