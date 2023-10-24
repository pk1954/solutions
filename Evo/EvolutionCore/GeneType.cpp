// GeneType.cpp : 
//
// EvolutionCore


#include "Genome.h"
#include "GeneType.h"

void GeneType::Apply2AllEnabledGeneTypes(function<void(Id const &)> const & func)
{
    for (int index = 0; index < static_cast<int>(Id::count); ++index)
	{
		Id const type { static_cast<Id>(index) };
		if (Genome::IsEnabled(GetRelatedAction(type)))
			func(type);
	}
}