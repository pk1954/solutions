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

unsigned long decFraction( wostringstream & wBuffer, unsigned long ulValue )
{
	unsigned long const intPlaces = ulValue / 1000;
	unsigned long const decPlaces = ulValue - 1000 * intPlaces;
	wBuffer << intPlaces << L".";
	return decPlaces;
}

std::wstring DecFraction( unsigned long ulValue )  // TODO: find better solution
{ 
	wostringstream wBuffer;
	unsigned long const decPlaces = decFraction( wBuffer, ulValue );

	unsigned long decPlace1 = decPlaces / 100;
	wBuffer << decPlace1;

	return wBuffer.str();
}

std::wstring DecFraction2( unsigned long ulValue )  // TODO: find better solution
{
	wostringstream wBuffer;
	unsigned long decPlaces = decFraction( wBuffer, ulValue );

	unsigned long decPlace1 = decPlaces / 100;
	wBuffer << decPlace1;

	unsigned long decPlace2 = (decPlaces - 100 * decPlace1) / 10;
	wBuffer << decPlace2;

	return wBuffer.str();
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
