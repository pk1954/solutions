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

wstring Format2wstring( fMicroSecs const us )
{
	wostringstream m_wstrBuffer;
	m_wstrBuffer << fixed << setprecision(2);
	if ( us > 1.e6_MicroSecs )  // more than one second
	{
		float seconds = us.GetValue() / 1000000.0f;
		m_wstrBuffer << seconds << L" s";
	}
	else if ( us > 1.e3_MicroSecs )  // more than one millisecond
	{
		float millisecs = us.GetValue() / 1000.0f;
		m_wstrBuffer << millisecs << L" ms";
	}
	else
	{
		float microsecs = us.GetValue();
		m_wstrBuffer << microsecs << L" << \u03BCs";
	}
	return m_wstrBuffer.str();
}

