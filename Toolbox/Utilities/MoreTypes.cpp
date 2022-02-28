// MoreTypes.cpp
//
// Utilities

#include "stdafx.h"
#include "MoreTypes.h"

int StepsOfThousand(float fValue)
{
	int iSteps { 0 };

	while (fValue < 1.f)
	{
		fValue *= 1e3f;
		++iSteps;
	}

	while (fValue >= 1000.f)
	{
		fValue *= 1e-3f;
		--iSteps;
	}

	return iSteps;
}

wstring GetUnitPrefix(int const iSteps)
{
	static wchar_t const prefix[]  { L'G', L'M', L'k', L' ', L'm', L'\u03BC', L'n', L'p' };
	static int     const iMaxIndex { sizeof(prefix)/sizeof(prefix[0]) - 1 };
	int iIndex { iSteps + 3 }; // 3 is index of L' '
	wchar_t const wchPrefix { IsInRange(iIndex, 0, iMaxIndex) ? prefix[iIndex] : L'?' };
	wstring wstrRes { L" " };
	if (wchPrefix != L' ')
		wstrRes += wchPrefix;
	return wstrRes;
}
