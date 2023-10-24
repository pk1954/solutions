// HistoryGeneration.cpp
//


#include "HistoryGeneration.h"

std::wostream & operator << (std::wostream & out, HistGeneration const & gen)
{
    out << L" " << gen.GetLong();
    return out;
}

std::ostream & operator << (std::ostream & out, HistGeneration const & gen)
{
    out << " " << gen.GetLong();
    return out;
}
