// util.cpp
//
// Utilities

#include "stdafx.h"
#include <sstream> 
#include "util.h"

using std::wstring;
using std::wostringstream;

extern void UpperCase(wstring & str)
{
	for (auto & c: str) 
		c = toupper(c);
}

bool ApplyAutoCriterion
(
	tOnOffAuto const   onOffAuto,
	VisCrit    const & crit
)
{
	if (onOffAuto == tOnOffAuto::automatic)
		return crit();
	else 
		return onOffAuto == tOnOffAuto::on;
}
