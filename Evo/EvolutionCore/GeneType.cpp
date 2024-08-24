// GeneType.cpp : 
//
// EvoCoreLib

module EvoCoreLib:GeneType;

import std;
import :Genome;
import :GeneType;

using std::function;

void GeneType::Apply2AllEnabledGeneTypes(function<void(Id const &)> const & func)
{
    for (int index = 0; index < static_cast<int>(Id::count); ++index)
	{
		Id const type { static_cast<Id>(index) };
		if (Genome::IsEnabled(GetRelatedAction(type)))
			func(type);
	}
}