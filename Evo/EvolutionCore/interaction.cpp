// Interaction.cpp
//
// EvolutionCore

module EvolutionCore:Interaction;

import :EvolutionTypes;
import Individual;
import Config;

static ENERGY_UNITS R;  // Beide kooperieren und fahren das gute Ergebnis R (Reward) ein. 
static ENERGY_UNITS S;  // Der Spieler wurde betrogen, er bekommt S (Suckers Payoff). 
static ENERGY_UNITS T;  // Der Spieler hat den anderen ausgenutzt und erhält T (Temptation).
static ENERGY_UNITS P;  // Gegenseitige Defektion führt nur zu P (Penalty). 

void INTERACTION::RefreshCash()
{
	R = ENERGY_UNITS(Config::GetConfigValueShort(Config::tId::interactionPayOff_R));
	S = ENERGY_UNITS(Config::GetConfigValueShort(Config::tId::interactionPayOff_S));
	T = ENERGY_UNITS(Config::GetConfigValueShort(Config::tId::interactionPayOff_T));
	P = ENERGY_UNITS(Config::GetConfigValueShort(Config::tId::interactionPayOff_P));
}

void INTERACTION::Interact(Individual &IndA, Individual &IndB, std::wostream * pOut)
{
	bool const resA = IndA.InteractWith(IndB.GetId());
	bool const resB = IndB.InteractWith(IndA.GetId());
    
	IndB.Remember(IndA.GetId(), resA);
	IndA.Remember(IndB.GetId(), resB);

	std::pair <ENERGY_UNITS,ENERGY_UNITS> const rewards =  
		resA ? (resB ? std::make_pair(R, R) : std::make_pair(S, T))
		     : (resB ? std::make_pair(T, S) : std::make_pair(P, P));
	
	IndA.IncEnergy(rewards.first);
	IndB.IncEnergy(rewards.second);

	if (pOut)
	{
		* pOut << L"   " << IndA.GetId() << "/" << IndB.GetId() << " "; 
		* pOut << "[" << Strategy::GetName(IndA.GetStrategyId()) << "]/";
		* pOut << "[" << Strategy::GetName(IndB.GetStrategyId()) << "]  "; 
		* pOut << resA << "/" << resB << "  ";
		* pOut << rewards.first.GetValue() << "/" << rewards.second.GetValue() << "  ";
		* pOut << IndA.GetEnergy().GetValue() << "/" << IndB.GetEnergy().GetValue();
		* pOut << std::endl;
	}
}
