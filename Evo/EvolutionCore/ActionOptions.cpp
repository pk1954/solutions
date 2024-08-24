// ActionOptions.cpp : 
//
// EvoCoreLib

module EvoCoreLib:ActionOptions;

import std;
import :Genome;

using std::endl;
using std::setw;
using std::setprecision;
using std::wostream;

void ActionOptions::set(Action::Id const action, bool const bValue)
{
	m_abOptions[action] = Genome::IsEnabled(action) && bValue;
}

void ActionOptions::InitOptions
(
	Genome       const & p,   
	bool         const   F,  //  hasFreeSpace,
    bool         const   N,  //  hasNeighbor,
    ENERGY_UNITS const   energy
)
{
	using A = Action::Id;
	using G = GeneType::Id;
	short E = energy.GetValue();
    set(A::move,      F &&      (E >= p.GetAllele(G::thresholdMove)));
	set(A::fertilize,           (E >= p.GetAllele(G::thresholdFert)));
	set(A::clone,     F &&      (E >= p.GetAllele(G::thresholdClone)));
	set(A::marry,     F && N && (E >= p.GetAllele(G::thresholdMarry)));
	set(A::interact,       N                                        );											 
	set(A::eat,                 (E <  p.GetAllele(G::maxEat       )));
	set(A::passOn,    false                                         );
}

void ActionOptions::DisplayValidOptions(wostream * const pOut, Genome const & genome, unsigned int const uiSum)
{
	* pOut << L"   valid options:" << endl;
	Action::Apply2AllEnabledActions
	(
		[&](auto action)
		{
			if (m_abOptions[action])
			{
				short const sAllele     = genome.GetAllele(GetRelatedGeneType(action));
				float const fPercentage = (uiSum == 0) ? 0 : (sAllele * 100.0f) / uiSum;
				* pOut << L"   " << setw(8) << Action::GetName(action) << L": " << setprecision(3) << fPercentage << L"%" << endl;
			}
		}
	);
}

// sum up all alleles of possible actions

unsigned int ActionOptions::GetSumOfValidOptions(Genome const & genome)
{
	unsigned int uiSum = 0;
    for (int index = 0; index < static_cast<int>(Action::Id::count); ++index)
	{
		Action::Id action = static_cast<Action::Id>(index);
		if (m_abOptions[action])
		{
			GeneType::Id const geneType = GetRelatedGeneType(action);
			if (GeneType::IsDefined(geneType))
			{
				short const sAllele = genome.GetAllele(geneType);
				uiSum += Cast2UnsignedInt(sAllele);
			}
		}
	}
	return uiSum;
}

// select randomly one possible action taking into account their relative probabilties

Action::Id ActionOptions::SelectAction(Genome const & genome, unsigned int const uiVal)
{
	int iVal = uiVal;
    for (int index = 0; index < static_cast<int>(Action::Id::count); ++index)
	{
		Action::Id action = static_cast<Action::Id>(index);
		if (m_abOptions[action])
		{
			GeneType::Id const geneType = GetRelatedGeneType(action);
			if (GeneType::IsDefined(geneType))
			{
				short const sAllele = genome.GetAllele(geneType);
				if ((iVal -= sAllele) <= 0)
					return action;  
			}
		}
	}
	return Action::Id::undefined;
}
