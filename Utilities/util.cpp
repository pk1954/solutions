// util.cpp
//

#include "stdafx.h"
#include <sstream> 
#include "util.h"

using std::wostringstream;

extern void UpperCase( std::wstring & str )
{
	for ( auto & c: str ) 
		c = toupper(c);
}

bool ApplyAutoCriterion
( 
	tOnOffAuto const onOffAuto,
	std::function<bool()> crit
)
{
	return ( onOffAuto == tOnOffAuto::on )
		? true
		: ( onOffAuto == tOnOffAuto::off )
		? false
		: crit();
}
