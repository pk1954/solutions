// HistoryGeneration.cpp
//

#include "stdafx.h"
#include "HistoryGeneration.h"

std::wostream & operator << ( std::wostream & out, HIST_GENERATION const & gen )
{
    out << L" " << gen.GetLong( );
    return out;
}

std::ostream & operator << ( std::ostream & out, HIST_GENERATION const & gen )
{
    out << " " << gen.GetLong( );
    return out;
}
