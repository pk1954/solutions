// MortalityTable.h : 
//
// EvolutionCore

module;

#include <array>
#include "random.h"
#include "EvolutionTypes.h"

class MortalityTable
{
public:
	static void InitClass();

	static unsigned int GetRate(EVO_GENERATION const age)
	{
		return m_mortalityTable[age.GetValue()];
	}

	static bool IsTimeToDie(EVO_GENERATION const age, Random & random)
	{
		return MortalityTable::GetRate(age) > random.NextRandomNumber();
	}

private:

	static unsigned int const MAX_LIFE_SPAN = 200;
	static std::array< unsigned int, MAX_LIFE_SPAN + 1 > m_mortalityTable;
};