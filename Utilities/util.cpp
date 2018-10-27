// util.cpp
//

#include "stdafx.h"
#include <sstream> 
#include "util.h"

extern void UpperCase( std::wstring & str )
{
	for ( auto & c: str ) 
		c = toupper(c);
}

std::wstring DecFraction( unsigned long ulValue )
{
    wostringstream wBuffer;
    unsigned long const intPlaces = ulValue / 1000;
    unsigned long const decPlaces = ulValue - 1000 * intPlaces;
    wBuffer << intPlaces << L"." << decPlaces / 100;
	return wBuffer.str();
}
