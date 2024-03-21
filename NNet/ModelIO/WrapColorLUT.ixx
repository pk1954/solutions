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
import NNetPreferences;

using std::endl;
using std::setw;
using std::setprecision;

export class WrapColorLUT : public Wrapper
{
public:
    WrapColorLUT()
     : Wrapper(NAME)
    {}

    void operator() (Script& script) const final
    {
        NNetPreferences::m_colorLUT.Clear();
        int    const iLutId   { script.ScrReadInt() };
        size_t const nrOfPnts { script.ScrReadUlong() };
        script.ScrReadSpecial(LIST_OPEN_BRACKET);
        for (size_t i = 0; i < nrOfPnts; ++i)
        {
            script.ScrReadSpecial(LIST_OPEN_BRACKET);
            ColIndex const colIndex { script.ScrReadUchar() };
            script.ScrReadSpecial(ID_SEPARATOR);
            Color    const color(ScrReadColor(script));
            NNetPreferences::m_colorLUT.AddBasePoint(colIndex, color);
            script.ScrReadSpecial(LIST_CLOSE_BRACKET);
        }
        script.ScrReadSpecial(LIST_CLOSE_BRACKET);
    }

    void Write(wostream& out) const final
    {
        int const iLutId = 1;
        out << NAME << SPACE << iLutId << SPACE << NNetPreferences::m_colorLUT.Size() << endl;
        out << LIST_OPEN_BRACKET << endl;
        NNetPreferences::m_colorLUT.Apply2AllBasePoints
        (
            [&out](BasePointNr const i)
            {
                out << SPACE;
                out << LIST_OPEN_BRACKET << setw(3) << NNetPreferences::m_colorLUT.GetColIndex(i);
                out << ID_SEPARATOR      << SPACE   << NNetPreferences::m_colorLUT.GetColor(i);
                out << LIST_CLOSE_BRACKET << endl;
            }
        );
        out << LIST_CLOSE_BRACKET << endl;
   }

private:
    inline static const wstring NAME { L"ColorLUT" };

};