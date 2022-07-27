// BasicTypes.cpp
//
// Utilities

#include <sstream>
#include <iomanip>
#include "BasicTypes.h"

using std::wostringstream;
using std::setprecision;
using std::fixed;

wstring Format2wstring(float fValue, streamsize prec)
{
	int iIndex { 0 };

	if (IsCloseToZero(fValue))
	{
		fValue = 0.f;
	}
	else 
	{
		int iSteps = StepsOfThousand(fValue);
		iIndex += iSteps;
		fValue *= powf(1000.f, static_cast<float>(iSteps));
	}

	wostringstream wstrBuffer;
	wstrBuffer << fixed << setprecision(prec) << fValue << GetUnitPrefix(iIndex);
	return wstrBuffer.str();
}
