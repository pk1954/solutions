// Interaction.ixx 
//
// EvolutionCore

export module InterAction;

import std;
import Individual;

using std::wostream;

export namespace INTERACTION
{
	void RefreshCash();
	void Interact(Individual &, Individual &, wostream *);
}
