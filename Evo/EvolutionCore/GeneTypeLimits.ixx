// GeneTypeLimits.ixx : 
//
// EvolutionCore

export module GeneTypeLimits;

import std;
import Util;
import Debug;
import SaveCast;

using std::max;

export class GeneTypeLimits
{
public:
	void SetLimits(long const lLo, long const lHi)
	{
		short const sLo = Cast2Short(lLo);
		short const sHi = Cast2Short(lHi);

		m_sMin = sLo;
		m_sMax = max(sHi, sLo);
		m_dFactor = (static_cast<double>(m_sMax) - static_cast<double>(m_sMin)) / 100.0;  // percentage
	}

	short ClipToLimits(short const sAllele) const
	{
		return ClipToMinMax(sAllele, m_sMin, m_sMax);
	}

	double GetFactor() const
	{
		return m_dFactor;
	};

	short  GetAverageValue() const
	{
		return (m_sMin + m_sMax) / 2;
	};

	void CheckLimits(short const sValue) const
	{
		AssertLimits(sValue, m_sMin, m_sMax);
	};

private:
	short  m_sMin;
	short  m_sMax;
	double m_dFactor;
};
