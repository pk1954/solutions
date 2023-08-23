// genome.cpp 
//
// EvolutionCore


#include "assert.h"
#include "debug.h"
#include "config.h"
#include "strategy.h"
#include "random.h"
#include "genome.h"

// static members and functions

Genome Genome::m_genomeTemplate;

EnumArray< bool,  Action  > Genome::m_abActionEnabled;
EnumArray< GeneTypeLimits, GeneType > Genome::m_aLimits;

void Genome::setLimits( GeneType::Id const gene, long const lLo, long const lHi )
{
    m_aLimits[ gene ].SetLimits( lLo, lHi );
}

void Genome::RefreshCash( )
{
	// initialize limitations

	setLimits( GeneType::Id::move,           1, 1000 ); 
	setLimits( GeneType::Id::clone,          1, 1000 ); 
	setLimits( GeneType::Id::marry,          1, 1000 ); 
	setLimits( GeneType::Id::interact,       1, 1000 );    
	setLimits( GeneType::Id::eat,            1, 1000 );    
	setLimits( GeneType::Id::fertilize,      1, 1000 );    
	setLimits( GeneType::Id::memSize,        1, IMEMSIZE_MAX );
	setLimits( GeneType::Id::appetite,       1, Config::GetConfigValue( Config::tId::maxFood      ) );
	setLimits( GeneType::Id::fertilInvest,   1, Config::GetConfigValue( Config::tId::maxFood      ) );
	setLimits( GeneType::Id::thresholdClone, 0, Config::GetConfigValue( Config::tId::stdCapacity  ) );
	setLimits( GeneType::Id::thresholdMarry, 0, Config::GetConfigValue( Config::tId::stdCapacity  ) );
	setLimits( GeneType::Id::thresholdMove,  0, Config::GetConfigValue( Config::tId::stdCapacity  ) );
	setLimits( GeneType::Id::thresholdFert,  0, Config::GetConfigValue( Config::tId::stdCapacity  ) );
	setLimits( GeneType::Id::maxEat,         0, Config::GetConfigValue( Config::tId::stdCapacity  ) );
	setLimits( GeneType::Id::cloneDonation,  0, SHRT_MAX                                           );

	// initialize genome template 

	m_genomeTemplate.setGene( GeneType::Id::move,           500 );
	m_genomeTemplate.setGene( GeneType::Id::clone,          500 );
	m_genomeTemplate.setGene( GeneType::Id::marry,          500 );
	m_genomeTemplate.setGene( GeneType::Id::interact,       500 );
	m_genomeTemplate.setGene( GeneType::Id::eat,            500 );
	m_genomeTemplate.setGene( GeneType::Id::fertilize,      500 );
	m_genomeTemplate.setGene( GeneType::Id::appetite,       Config::GetConfigValueShort( Config::tId::defaultAppetite     ) );
	m_genomeTemplate.setGene( GeneType::Id::fertilInvest,   Config::GetConfigValueShort( Config::tId::defaultFertilInvest ) );
	m_genomeTemplate.setGene( GeneType::Id::memSize,        Config::GetConfigValueShort( Config::tId::stdMemSize          ) );
	m_genomeTemplate.setGene( GeneType::Id::thresholdClone, Config::GetConfigValueShort( Config::tId::thresholdClone      ) );
	m_genomeTemplate.setGene( GeneType::Id::thresholdMarry, Config::GetConfigValueShort( Config::tId::thresholdMarry      ) );
	m_genomeTemplate.setGene( GeneType::Id::thresholdMove,  Config::GetConfigValueShort( Config::tId::thresholdMove       ) );
	m_genomeTemplate.setGene( GeneType::Id::thresholdFert,  Config::GetConfigValueShort( Config::tId::thresholdFert       ) );
	m_genomeTemplate.setGene( GeneType::Id::maxEat,         Config::GetConfigValueShort( Config::tId::maxEat              ) );
	m_genomeTemplate.setGene( GeneType::Id::cloneDonation,  SHRT_MAX / 2 );

	m_abActionEnabled[ Action::Id::move      ] = Config::GetConfigValueBool( Config::tId::moveEnabled      );
	m_abActionEnabled[ Action::Id::clone     ] = Config::GetConfigValueBool( Config::tId::cloneEnabled     );
	m_abActionEnabled[ Action::Id::marry     ] = Config::GetConfigValueBool( Config::tId::marryEnabled     );
	m_abActionEnabled[ Action::Id::interact  ] = Config::GetConfigValueBool( Config::tId::interactEnabled  );
	m_abActionEnabled[ Action::Id::eat       ] = Config::GetConfigValueBool( Config::tId::eatEnabled       );
	m_abActionEnabled[ Action::Id::fertilize ] = Config::GetConfigValueBool( Config::tId::fertilizeEnabled );
	m_abActionEnabled[ Action::Id::passOn    ] = Config::GetConfigValueBool( Config::tId::passOnEnabled    );

}

void Genome::InitClass( )
{ 
	RefreshCash( );  // static members for caching frequently used configuration items
}

//  nonstatic functions 

Genome::Genome( )
{
    InitGenome( );
}

void Genome::InitGenome( )
{
    if ( this != & m_genomeTemplate )
        * this = m_genomeTemplate;
}

void Genome::Mutate( PERCENT const mutationRate, Random & random )
{
	double dMutationRate = static_cast<double>(mutationRate.GetValue());

	GeneType::Apply2AllEnabledGeneTypes
	(
		[&]( auto geneType )
		{
			m_aGene[geneType].Mutate( dMutationRate, m_aLimits[geneType], random );
		}
	);
}

void Genome::Recombine( Genome const & genomeA, Genome const & genomeB, Random & random )
{
	GeneType::Apply2AllEnabledGeneTypes
	(
		[&]( auto geneType )
		{
			Genome genome     = random.NextBooleanValue( ) ? genomeA : genomeB;
			short  sNewAllele = GetAllele(geneType);
			m_aGene[geneType].SetAllele( sNewAllele );
		}
	);
}
