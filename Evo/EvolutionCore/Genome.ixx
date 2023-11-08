// Genome.ixx
//
// EvolutionCore

module;

#include "EnumArray.h"

export module Genome;

import Types;
import Gene;
import EvolutionTypes;

class Random;

export class Genome
{
public:

	Genome();
	void InitGenome();
	void Mutate(PERCENT const, Random&);
	void Recombine(Genome const&, Genome const&, Random&);

	static void InitClass();
	static void RefreshCash();

	// for statistics

	short GetAllele(GeneType::Id const geneType) const
	{
		return m_aGene[geneType].GetAllele();
	};

	static bool IsEnabled(Action::Id const action)
	{
		return m_abActionEnabled[action];;
	};

private:

	EnumArray< Gene, GeneType > m_aGene;

	void setGene(GeneType::Id const type, short const sValue)
	{
		m_aLimits[type].CheckLimits(sValue);
		m_aGene[type].SetAllele(sValue);
	}

	// static members and functions

	static EnumArray<          bool, Action>   m_abActionEnabled;
	static EnumArray<GeneTypeLimits, GeneType> m_aLimits;
	static Genome                              m_genomeTemplate;

	static void setLimits(GeneType::Id, long, long);
};
