// util.cpp
//

#include "stdafx.h"
#include "util.h"

extern void UpperCase( std::wstring & str )
{
	for ( auto & c: str ) 
		c = toupper(c);
}

extern short ClipToMinMax(short sVal, short const sMin, short const sMax)
{
	return ( sVal > sMax ) 
		   ? sMax 
		   : (sVal < sMin) 
		     ? sMin 
		     : sVal;
}

extern short ClipToMin(short sVal, short const sMin)
{
	return (sVal < sMin) 
		   ? sMin 
		   : sVal;
}

extern short ClipToMax(short sVal, short const sMax)
{
	return ( sVal > sMax ) 
		   ? sMax 
		   : sVal;
}
