// Interaction.ixx 
//
// EvolutionCore

module;

#include <iostream>

export module InterAction;

class Individual;

export namespace INTERACTION
{
	void RefreshCash();
	void Interact(Individual &, Individual &, std::wostream *);
}
