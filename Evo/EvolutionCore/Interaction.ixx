// Interaction.ixx 
//
// EvoCoreLib

export module EvoCoreLib:Interaction;

import std;
import :Individual;

using std::wostream;

export class Interaction
{
public:
	static void RefreshCash();
	static void Interact(Individual&, Individual&, wostream*);
};
