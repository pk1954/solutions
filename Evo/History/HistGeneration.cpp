// HistGeneration.cpp
//
// History

module HistoryLib:HistGeneration;

import std;

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
