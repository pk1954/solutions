// IoBasePeak.ixx
//
// Toolbox\IoUtil

export module IoUtil:IoBasePeak;

import std;
import Types;
import RunTime;
import BasePeak;
import IoConstants;

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
        << SPACE
        << data.Peak().GetValue()
        << SPACE;
    return out;
}
