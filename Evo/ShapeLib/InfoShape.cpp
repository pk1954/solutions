// InfoShape.cpp
//
// ShapeLib

module ShapeLib:InfoShape;

#include "win32_infoShape.h"

import Config;

using std::wostringstream;
using std::setw;
using std::endl;

void InfoShape::FillBuffer(EvolutionCore const * const pCore, GridPoint const gp)
{
	IND_ID          id       { 0x12345678 };
	ENERGY_UNITS    energy   { 12345 };
	EVO_GENERATION  evoGen   { 0 };
	wchar_t const * origin   { L"" };
	wchar_t const * strategy { L"" };

	if (gp.IsNotZero())
	{
		id       = pCore->GetId          (gp);
		energy   = pCore->GetEnergy      (gp);
		evoGen   = pCore->GetAge         (gp);
		origin   = pCore->GetOriginName  (gp);
		strategy = pCore->GetStrategyName(gp);
	}

	wostringstream & buffer = m_pTextDisplay->Buffer();
	buffer << id                                        << endl;
	buffer << L"En: " << setw(5) << energy.GetValue() << endl;
	buffer << L"Age:" << setw(5) << evoGen.GetValue() << endl;
	buffer << L"Or: " << setw(5) << origin            << endl;
	buffer << L"Str:" << setw(5) << strategy;
}
