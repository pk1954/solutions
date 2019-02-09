// interaction.cpp
//

#include "stdafx.h"
#include <iostream>
#include "config.h"
#include "EvolutionTypes.h"
#include "individual.h"
#include "interaction.h"

static short R;  // Beide kooperieren und fahren das gute Ergebnis R (Reward) ein. 
static short S;  // Der Spieler wurde betrogen, er bekommt S (Suckers Payoff). 
static short T;  // Der Spieler hat den anderen ausgenutzt und erhält T (Temptation).
static short P;  // Gegenseitige Defektion führt nur zu P (Penalty). 

void INTERACTION::InitClass( )
{
	R = Config::GetConfigValueShort( Config::tId::interactionPayOff_R );
	S = Config::GetConfigValueShort( Config::tId::interactionPayOff_S );
	T = Config::GetConfigValueShort( Config::tId::interactionPayOff_T );
	P = Config::GetConfigValueShort( Config::tId::interactionPayOff_P );
}

void INTERACTION::Interact( Individual &IndA, Individual &IndB )
{
	bool const resA = IndA.InteractWith( IndB.GetId() );
	bool const resB = IndB.InteractWith( IndA.GetId() );
    
	IndB.Remember( IndA.GetId(), resA );
	IndA.Remember( IndB.GetId(), resB );

	std::pair <short,short> const rewards =  
		resA ? (resB ? std::make_pair( R, R ) : std::make_pair( S, T ))
		     : (resB ? std::make_pair( T, S ) : std::make_pair( P, P ));
	
	IndA.IncEnergy( rewards.first );
	IndB.IncEnergy( rewards.second );

	int const DUMP = 0;

	if /*lint -e774 */ ( DUMP )
	{
		std::cout << IndA.GetId().GetLong() << "[" << GetStrategyName( IndA.GetStrategyId() ) << "]/";
		std::cout << IndB.GetId().GetLong() << "[" << GetStrategyName( IndB.GetStrategyId() ) << "]  "; 
		std::cout << resA << "/" << resB << "  ";
		std::cout << rewards.first << "/" << rewards.second << "  ";
		std::cout << IndA.GetEnergy() << "/" << IndB.GetEnergy();
		std::cout << std::endl;
	}
}
