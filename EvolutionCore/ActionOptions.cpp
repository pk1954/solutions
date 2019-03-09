// ActionOptions.cpp : 
//
// EvolutionCore

#include "stdafx.h"
#include "genome.h"
#include "ActionOptions.h"

void ActionOptions::set( Action::Id const action, bool const bValue )
{
	abOptions.at( static_cast<int>( action ) ) = Genome::IsEnabled( action ) && bValue;
}

void ActionOptions::InitOptions
( 
	Genome       const * p,   
	bool         const   bHasFreeSpace, 
    bool         const   bHasNeighbor,
    ENERGY_UNITS const   energy
)
{
	using A  = Action::Id;
	using GT = GeneType::Id;
	short en = energy.GetValue();
    set( A::move,      bHasFreeSpace &&                 (en >= p->GetAllele( GT::thresholdMove) ));
	set( A::fertilize,                                  (en >= p->GetAllele( GT::thresholdFert) ));
	set( A::clone,     bHasFreeSpace &&                 (en >= p->GetAllele( GT::thresholdClone)));
	set( A::marry,     bHasFreeSpace && bHasNeighbor && (en >= p->GetAllele( GT::thresholdMarry)));
	set( A::interact,                   bHasNeighbor );											 
	set( A::eat,                                        (en <  p->GetAllele( GT::maxEat)        ));
	set( A::passOn, false );
}

// sum up all alleles of posible actions

unsigned int ActionOptions::GetSumOfValidOptions( Genome const * pGenome )
{
	unsigned int uiSum = 0;

	Action::Apply2All  
	(
		[&]( Action::Id action ) -> Action::Id
		{
			if ( IsPossible( action ) )
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
	return Action::Apply2All
	(
		[&]( Action::Id action ) -> Action::Id 
		{
			if ( IsPossible( action ) )
			{
				if ( (iVal -= pGenome->GetAllele( action )) <= 0 )
				{
					return action;         // break out of Apply2All loop
				}
			}
			return Action::Id::undefined;  // continue in loop 
		}
	);
}
