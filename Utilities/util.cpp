// util.cpp
//

#include "stdafx.h"
#include "util.h"

extern void UpperCase( std::wstring & str )
{
	for ( auto & c: str ) 
		c = toupper(c);
}
