// Util.cpp
//
// Utilities

module;

#include <string>
#include <sstream>
#include <iomanip>

module Util;

using std::wstring;
using std::wostringstream;
using std::streamsize;
using std::setprecision;
using std::fixed;

void UpperCase(wstring & str)
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
