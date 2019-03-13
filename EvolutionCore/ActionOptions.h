// ActionOptions.h : 
//
// EvolutionCore

#pragma once

#include <array>
#include "random.h"
#include "EnumArray.h"
#include "EvolutionTypes.h"

class Genome;

class ActionOptions
{
public:
	void InitOptions( Genome const *, bool const, bool const, ENERGY_UNITS const );

	unsigned int GetSumOfValidOptions( Genome const *  );

	Action::Id SelectAction( Genome const *, int );

private:
	void set( Action::Id const, bool const );

	EnumArray<bool, Action> m_abOptions;
};
