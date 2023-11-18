// HistGeneration.cpp
//
// History

module;

#include <iostream>

module HistGeneration;

using std::wostream;
using std::ostream;

wostream & operator << (wostream & out, HistGeneration const & gen)
{
    out << L" " << gen.GetLong();
    return out;
}

ostream & operator << (ostream & out, HistGeneration const & gen)
{
    out << " " << gen.GetLong();
    return out;
}
