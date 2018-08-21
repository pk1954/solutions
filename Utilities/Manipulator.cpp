// Manipulator.cpp
//

#include "stdafx.h"
#include <unordered_map>
#include "Manipulator.h"

using namespace std;

wchar_t const * const GetOperatorName( tOperator const op )
{
    static unordered_map < tOperator, wchar_t const * const > mapOperators =
    { 
        { tOperator::set,      L"set" },
        { tOperator::max,      L"max" },
        { tOperator::min,      L"min" },
        { tOperator::add,      L"add" },
        { tOperator::subtract, L"subtract" },
        { tOperator::mean,     L"mean" },
    };

    return mapOperators.at( op );
}

