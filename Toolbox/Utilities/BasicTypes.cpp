// BasicTypes.cpp
//
// Utilities

module;

#include <string>
#include <sstream>
#include <iomanip>

module Types:BasicTypes;

using std::wstring;
using std::wostringstream;
using std::streamsize;
using std::setprecision;
using std::fixed;

void UpperCase(wstring& str)
{
	for (auto& c : str)
		c = toupper(c);
}

wstring Float2wstring(float fValue, streamsize prec)
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
	static wchar_t const prefix[] { L'G', L'M', L'k', L' ', L'm', L'\u03BC', L'n', L'p' };
	static int     const iMaxIndex { sizeof(prefix) / sizeof(prefix[0]) - 1 };
	int iIndex { iSteps + 3 }; // 3 is index of L' '
	wchar_t const wchPrefix { IsInRange(iIndex, 0, iMaxIndex) ? prefix[iIndex] : L'?' };
	wstring wstrRes { L" " };
	if (wchPrefix != L' ')
		wstrRes += wchPrefix;
	return wstrRes;
}
