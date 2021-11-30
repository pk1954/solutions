// MoreTypes.cpp
//
// Utilities

#include "stdafx.h"
#include <sstream>
#include <iomanip>
#include "MoreTypes.h"

using std::wostringstream;
using std::setprecision;
using std::fixed;

int StepsOfThousand(float fValue)
{
	int iSteps { 0 };
	//int iSteps2 { 0 };		
	//float fLog1000;
	//float fFloor;

	//if ( fValue < 1.f )
	//{
	//	fLog1000 = log10(fValue) / 3.f;
	//	fFloor   = floor(-fLog1000);
	//	iSteps2  = -static_cast<int>(fFloor);
	//}
	//else
	//{
	//	fLog1000 = log10(fValue) / 3.f;
	//	fFloor   = floor(fLog1000);
	//	iSteps2  = static_cast<int>(fFloor);
	//}

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

	//if ( iSteps != iSteps2 )
	//{
	//	int x = 42;
	//}

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

wstring Format2wstring(float fValue)
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
	wstrBuffer << fixed << setprecision(2) << fValue << GetUnitPrefix(iIndex);
	return wstrBuffer.str();
}
