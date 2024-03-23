// WrapColorLUT.ixx
//
// ModelIO

module;

#include <iostream>
#include <iomanip>
#include <string>
#include <Windows.h>

export module WrapColorLUT;

import Wrapper;
import ColorLUT;
import Color;
import IoUtil;

using std::endl;
using std::setw;
using std::setprecision;

export class WrapColorLUT : public Wrapper
{
public:
    WrapColorLUT(ColorLUT &lut)
     : Wrapper(NAME),
       m_lut(lut)
    {}

    void operator() (Script& script) const final
    {
        m_lut.Clear();
        int    const iLutId   { script.ScrReadInt() };
        size_t const nrOfPnts { script.ScrReadUlong() };
        script.ScrReadSpecial(LIST_OPEN_BRACKET);
        for (size_t i = 0; i < nrOfPnts; ++i)
        {
            script.ScrReadSpecial(LIST_OPEN_BRACKET);
            ColIndex const colIndex { script.ScrReadUchar() };
            script.ScrReadSpecial(ID_SEPARATOR);
            Color    const color(ScrReadColor(script));
            m_lut.AddBasePoint(colIndex, color);
            script.ScrReadSpecial(LIST_CLOSE_BRACKET);
        }
        script.ScrReadSpecial(LIST_CLOSE_BRACKET);
    }

    void Write(wostream& out) const final
    {
        int const iLutId = 1;
        out << NAME << SPACE << iLutId << SPACE << m_lut.Size() << endl;
        out << LIST_OPEN_BRACKET << endl;
        m_lut.Apply2AllBasePoints
        (
            [this, &out](BasePointNr const i)
            {
                out << SPACE;
                out << LIST_OPEN_BRACKET << setw(3) << m_lut.GetColIndex(i);
                out << ID_SEPARATOR      << SPACE   << m_lut.GetColor(i);
                out << LIST_CLOSE_BRACKET << endl;
            }
        );
        out << LIST_CLOSE_BRACKET << endl;
   }

private:

    inline static const wstring NAME { L"ColorLUT" };

    ColorLUT& m_lut;
};