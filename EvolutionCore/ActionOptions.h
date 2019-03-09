// ActionOptions.h : 
//
// EvolutionCore

#pragma once

#include <array>
#include "random.h"
#include "EvolutionTypes.h"

class Genome;

class ActionOptions
{
public:
	void InitOptions( Genome const *, bool const, bool const, ENERGY_UNITS const );

	unsigned int GetSumOfValidOptions( Genome const *  );

	Action::Id SelectAction( Genome const *, int );

	bool IsPossible( Action::Id const action )
	{
		return abOptions.at( static_cast<int>( action ) );
	}

private:
	void set( Action::Id const, bool const );

	std::array <bool, Action::COUNT> abOptions;
};
