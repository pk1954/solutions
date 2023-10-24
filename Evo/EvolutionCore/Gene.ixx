// Gene.ixx
//
// EvolutionCore

module;

#include <cassert>

export module Gene;

import Random;
import GeneTypeLimits;

class Gene
{
public:
	Gene()
		: m_sAllele(0)
	{ }

	explicit Gene(GeneTypeLimits const& lim)
		: m_sAllele(lim.GetAverageValue())
	{ }

	void Mutate
	(
		double         const   dMutationRate,
		GeneTypeLimits const& lim,
		Random& random
	)
	{
		assert(dMutationRate <= 100);   // percentage

		double const dRand = random.NextWeightedDistribution() * lim.GetFactor();

		m_sAllele = lim.ClipToLimits(CastToInt(m_sAllele + dRand * dMutationRate));
	}

	void SetAllele(short const sValue)
	{
		m_sAllele = sValue;
	};

	short GetAllele() const
	{
		return m_sAllele;
	}

private:

	short m_sAllele;
};
