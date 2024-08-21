// ActionOptions.ixx
//
// EvolutionCore

export module ActionOptions;

import std;
import Random;
import EnumArray;
import EvolutionTypes;
import Action;
import Genome;

using std::wostream;

export class ActionOptions
{
public:
	void InitOptions(Genome const &, bool const, bool const, ENERGY_UNITS const);

	void DisplayValidOptions(wostream * const, Genome const &, unsigned int const);

	unsigned int GetSumOfValidOptions(Genome const &);

	Action::Id SelectAction(Genome const &, unsigned int const);

private:
	void set(Action::Id const, bool const);

	EnumArray<bool, Action> m_abOptions;
};
