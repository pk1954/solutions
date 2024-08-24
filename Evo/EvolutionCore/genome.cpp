// Genome.cpp 
//
// EvoCoreLib

module EvoCoreLib:Genome;

import Debug;
import Random;
import :EvoConfig;
import :Strategy;

using std::numeric_limits;

// static members and functions

Genome Genome::m_genomeTemplate;

EnumArray< bool,  Action  > Genome::m_abActionEnabled;
EnumArray< GeneTypeLimits, GeneType > Genome::m_aLimits;

void Genome::setLimits(GeneType::Id const gene, long const lLo, long const lHi)
{
    m_aLimits[gene].SetLimits(lLo, lHi);
}

void Genome::RefreshCash()
{
	// initialize limitations

	setLimits(GeneType::Id::move,           1, 1000); 
	setLimits(GeneType::Id::clone,          1, 1000); 
	setLimits(GeneType::Id::marry,          1, 1000); 
	setLimits(GeneType::Id::interact,       1, 1000);    
	setLimits(GeneType::Id::eat,            1, 1000);    
	setLimits(GeneType::Id::fertilize,      1, 1000);    
	setLimits(GeneType::Id::memSize,        1, IMEMSIZE_MAX);
	setLimits(GeneType::Id::appetite,       1, EvoConfig::GetConfigValue(EvoConfig::tId::maxFood     ));
	setLimits(GeneType::Id::fertilInvest,   1, EvoConfig::GetConfigValue(EvoConfig::tId::maxFood     ));
	setLimits(GeneType::Id::thresholdClone, 0, EvoConfig::GetConfigValue(EvoConfig::tId::stdCapacity ));
	setLimits(GeneType::Id::thresholdMarry, 0, EvoConfig::GetConfigValue(EvoConfig::tId::stdCapacity ));
	setLimits(GeneType::Id::thresholdMove,  0, EvoConfig::GetConfigValue(EvoConfig::tId::stdCapacity ));
	setLimits(GeneType::Id::thresholdFert,  0, EvoConfig::GetConfigValue(EvoConfig::tId::stdCapacity ));
	setLimits(GeneType::Id::maxEat,         0, EvoConfig::GetConfigValue(EvoConfig::tId::stdCapacity ));
	setLimits(GeneType::Id::cloneDonation,  0, numeric_limits<short>::max());

	// initialize genome template 

	m_genomeTemplate.setGene(GeneType::Id::move,           500);
	m_genomeTemplate.setGene(GeneType::Id::clone,          500);
	m_genomeTemplate.setGene(GeneType::Id::marry,          500);
	m_genomeTemplate.setGene(GeneType::Id::interact,       500);
	m_genomeTemplate.setGene(GeneType::Id::eat,            500);
	m_genomeTemplate.setGene(GeneType::Id::fertilize,      500);
	m_genomeTemplate.setGene(GeneType::Id::appetite,       EvoConfig::GetConfigValueShort(EvoConfig::tId::defaultAppetite    ));
	m_genomeTemplate.setGene(GeneType::Id::fertilInvest,   EvoConfig::GetConfigValueShort(EvoConfig::tId::defaultFertilInvest));
	m_genomeTemplate.setGene(GeneType::Id::memSize,        EvoConfig::GetConfigValueShort(EvoConfig::tId::stdMemSize         ));
	m_genomeTemplate.setGene(GeneType::Id::thresholdClone, EvoConfig::GetConfigValueShort(EvoConfig::tId::thresholdClone     ));
	m_genomeTemplate.setGene(GeneType::Id::thresholdMarry, EvoConfig::GetConfigValueShort(EvoConfig::tId::thresholdMarry     ));
	m_genomeTemplate.setGene(GeneType::Id::thresholdMove,  EvoConfig::GetConfigValueShort(EvoConfig::tId::thresholdMove      ));
	m_genomeTemplate.setGene(GeneType::Id::thresholdFert,  EvoConfig::GetConfigValueShort(EvoConfig::tId::thresholdFert      ));
	m_genomeTemplate.setGene(GeneType::Id::maxEat,         EvoConfig::GetConfigValueShort(EvoConfig::tId::maxEat             ));
	m_genomeTemplate.setGene(GeneType::Id::cloneDonation,  numeric_limits<short>::max() / 2);

	m_abActionEnabled[Action::Id::move     ] = EvoConfig::GetConfigValueBool(EvoConfig::tId::moveEnabled     );
	m_abActionEnabled[Action::Id::clone    ] = EvoConfig::GetConfigValueBool(EvoConfig::tId::cloneEnabled    );
	m_abActionEnabled[Action::Id::marry    ] = EvoConfig::GetConfigValueBool(EvoConfig::tId::marryEnabled    );
	m_abActionEnabled[Action::Id::interact ] = EvoConfig::GetConfigValueBool(EvoConfig::tId::interactEnabled );
	m_abActionEnabled[Action::Id::eat      ] = EvoConfig::GetConfigValueBool(EvoConfig::tId::eatEnabled      );
	m_abActionEnabled[Action::Id::fertilize] = EvoConfig::GetConfigValueBool(EvoConfig::tId::fertilizeEnabled);
	m_abActionEnabled[Action::Id::passOn   ] = EvoConfig::GetConfigValueBool(EvoConfig::tId::passOnEnabled   );

}

void Genome::InitClass()
{ 
	RefreshCash();  // static members for caching frequently used configuration items
}

//  nonstatic functions 

Genome::Genome()
{
    InitGenome();
}

void Genome::InitGenome()
{
    if (this != & m_genomeTemplate)
        * this = m_genomeTemplate;
}

void Genome::Mutate(PERCENT const mutationRate, Random & random)
{
	double dMutationRate = static_cast<double>(mutationRate.GetValue());

	GeneType::Apply2AllEnabledGeneTypes
	(
		[&](auto geneType)
		{
			m_aGene[geneType].Mutate(dMutationRate, m_aLimits[geneType], random);
		}
	);
}

void Genome::Recombine(Genome const & genomeA, Genome const & genomeB, Random & random)
{
	GeneType::Apply2AllEnabledGeneTypes
	(
		[&](auto geneType)
		{
			Genome genome     = random.NextBooleanValue() ? genomeA : genomeB;
			short  sNewAllele = GetAllele(geneType);
			m_aGene[geneType].SetAllele(sNewAllele);
		}
	);
}
