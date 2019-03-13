// ActionOptions.cpp : 
//
// EvolutionCore

#include "stdafx.h"
#include "genome.h"
#include "ActionOptions.h"

void ActionOptions::set( Action::Id const action, bool const bValue )
{
	m_abOptions[action] = Genome::IsEnabled( action ) && bValue;
}

void ActionOptions::InitOptions
( 
	Genome       const * p,   
	bool         const   F,  //  hasFreeSpace,
    bool         const   N,  //  hasNeighbor,
    ENERGY_UNITS const   energy
)
{
	using A = Action::Id;
	using G = GeneType::Id;
	short E = energy.GetValue();
    set(A::move,      F &&      (E >= p->GetAllele(G::thresholdMove )));
	set(A::fertilize,           (E >= p->GetAllele(G::thresholdFert )));
	set(A::clone,     F &&      (E >= p->GetAllele(G::thresholdClone)));
	set(A::marry,     F && N && (E >= p->GetAllele(G::thresholdMarry)));
	set(A::interact,       N                                          );											 
	set(A::eat,                 (E <  p->GetAllele(G::maxEat        )));
	set(A::passOn,    false                                           );
}

// sum up all alleles of posible actions

unsigned int ActionOptions::GetSumOfValidOptions( Genome const * pGenome )
{
	unsigned int uiSum = 0;

	Action::Apply2All  
	(
		[&]( Action::Id action ) -> Action::Id
		{
			if ( m_abOptions[action] )
			{
				uiSum += CastToUnsignedInt( pGenome->GetAllele( action ) );
			}
			return Action::Id::undefined;
		}
	);

	return uiSum;
}

// select randomly one possible action taking into account their relative probabilties

Action::Id ActionOptions::SelectAction( Genome const * pGenome, int iVal )
{
	Action::Id const actionRes = Action::Select
	(
		[&]( Action::Id action ) -> Action::Id 
		{
			if ( m_abOptions[action] )
			{
				if ( (iVal -= pGenome->GetAllele( action )) <= 0 )
				{
					return action;         // break out of Apply2All loop
				}
			}
			return Action::Id::undefined;  // continue in loop 
		}
	);
	return actionRes;
}
