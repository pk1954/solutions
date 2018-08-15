// util.h : 
//

#pragma once

#include <string>
#include <streambuf>

using namespace std;

extern void UpperCase( wstring & );

extern short ClipToMinMax(short, short const, short const);
extern short ClipToMin(short, short const);
extern short ClipToMax(short, short const);
