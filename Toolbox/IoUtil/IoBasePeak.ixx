// IoBasePeak.ixx
//
// Toolbox\IoUtil

module;

#include <iostream>
#include <iomanip>

export module IoUtil:IoBasePeak;

import Types;
import Script;
import BasePeak;

using std::wostream;
using std::setw;

export template<typename T>
BasePeak<T> ScrReadBasePeak(Script& script)
{
    T const base { ScrRead<T>(script) };
    T const peak { ScrRead<T>(script) };
    return BasePeak<T>(base, peak);
}

export template<typename T>
wostream& operator<< (wostream& out, BasePeak<T> const& data)
{
    out << setw(5)
        << data.Base().GetValue()
        << L' '
        << data.Peak().GetValue()
        << L' ';
    return out;
}
