// EvolutionTypes.cpp
//

#include "stdafx.h"
#include "IndividualId.h"

const IndividualId IndividualId::NO_INDIVIDUAL = IndividualId();

std::wostream & operator << ( std::wostream & out, IndividualId const & id )
{
    out << id.GetLong( );
    return out;
};
