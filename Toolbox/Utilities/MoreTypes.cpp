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

void Format2wstring(fMicroSecs const us, wstring & wstr)
{
	wostringstream wstrBuffer;
	wstrBuffer << fixed << setprecision(2);
	if (us > 1.e6_MicroSecs)  // more than one second
	{
		float seconds = us.GetValue() / 1000000.0f;
		wstrBuffer << seconds << L" s";
	}
	else if (us > 1.e3_MicroSecs)  // more than one millisecond
	{
		float millisecs = us.GetValue() / 1000.0f;
		wstrBuffer << millisecs << L" ms";
	}
	else
	{
		float microsecs = us.GetValue();
		wstrBuffer << microsecs << L" \u03BCs";
	}
	wstr = wstrBuffer.str();
}

