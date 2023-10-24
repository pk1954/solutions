// MortalityTable.cpp 
//
// EvolutionCore


#include "MortalityTable.h"

std::array< unsigned int, MortalityTable::MAX_LIFE_SPAN + 1 > MortalityTable::m_mortalityTable;

void MortalityTable::InitClass()
{ 
	double dRandomMax = static_cast<double>(Random::MAX_VAL);

	for (int age = 0; age <= MAX_LIFE_SPAN; ++age)
	{
		double dAge = static_cast<double>(age);
		double dx   = dAge / MAX_LIFE_SPAN;
		double dx2  = dx * dx;
		double dx4  = dx2 * dx2;
		double dx8  = dx4 * dx4;
		double dAgeFactor = dx8 * dRandomMax;
		AssertLimits(dAgeFactor, 0.0, dRandomMax);
		m_mortalityTable[age] = static_cast<unsigned int>(dAgeFactor);
	}
}
