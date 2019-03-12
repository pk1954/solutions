// GeneType.h
//
// EvolutionCoreInterface

#pragma once

#include <array>
#include <functional>

class GeneType
{
public:
	enum class Id : unsigned short
	{
		move, 
		clone,
		marry,
		interact, 
		eat, 
		fertilize,
		appetite,
		fertilInvest,
		memSize,
		thresholdClone,  // minimum available energy for considering CLONE
		thresholdMarry,  // minimum available energy for considering MARRY
		thresholdMove,   // minimum available energy for considering MOVE
		thresholdFert,   // minimum available energy for considering FERTILIZE
		maxEat,          // maximum available energy for considering EAT
		cloneDonation,   // amount of energy donated to clone. 0 means nothing (clone dies), SHORT_MAX means all available energy (parent dies)
		count,
		undefined = count
	};

	static const int COUNT = static_cast<int>( Id::count );

	static void Apply2All( std::function<void(Id const &)> const & func )
	{
        for ( int index = 0; index < COUNT; ++index )
		{
            func( static_cast<Id>(index) );
		}
	}

	static bool IsDefined( Id const geneType )
	{
		return geneType != Id::undefined;
	}

	static bool IsUndefined( Id const geneType )
	{
		return geneType == Id::undefined;
	}

	static wchar_t const * const GetName( Id const );
};
