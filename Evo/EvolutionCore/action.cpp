// Action.cpp : 
//
// EvolutionCore

module EvolutionCore:Action;

import std;
import :Genome;

using std::function;

void Action::Apply2AllEnabledActions(function<void(Id const &)> const & func)
{
    for (int index = 0; index < static_cast<int>(Id::count); ++index)
	{
		Id const action { static_cast<Id>(index) };
	 	if (Genome::IsEnabled(action))
		    func(static_cast<Id>(index));   
	}
}
