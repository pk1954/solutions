// interaction.h : 
//

#pragma once

#include <iostream>

class Individual;

namespace INTERACTION
{
	void RefreshCash( );
	void Interact( Individual &, Individual &, std::wostream * );
}
