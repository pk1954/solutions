// Gene.ixx
//
// EvolutionCore

export module EvolutionCore:Gene;

import Debug;
import Random;
import SaveCast;
import :GeneTypeLimits;

export class Gene
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
		Assert(dMutationRate <= 100);   // percentage

		double const dRand = random.NextWeightedDistribution() * lim.GetFactor();

		m_sAllele = lim.ClipToLimits(Cast2Int(m_sAllele + dRand * dMutationRate));
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
