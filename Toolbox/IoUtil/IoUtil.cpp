// IoUtil.cpp
//
// Toolbox\IoUtil

module;

#include <cassert>
#include <compare>
#include <iostream>
#include <iomanip>
#include <Windows.h>

module IoUtil;

import SaveCast;
import BoolOp;
import Trace;
import Types;
import Symtab;
import Script;
import IoConstants;

using std::wostream;
using std::setprecision;

wostream& operator<< (wostream& out, fPixelPoint const& pnt)
{
    out << SPACE
        << OPEN_BRACKET
        << setprecision(10)
        << pnt.GetX()
        << SEPARATOR
        << pnt.GetY()
        << CLOSE_BRACKET;
    return out;
}

wostream& operator<< (wostream& out, MicroMeterPnt const& pnt)
{
    out << SPACE
        << OPEN_BRACKET
        << setprecision(10)
        << pnt.GetX()
        << SEPARATOR
        << pnt.GetY()
        << CLOSE_BRACKET;
    return out;
}

wostream& operator<< (wostream& out, MicroMeterRect const& rect)
{
    out << rect.GetStartPoint() << rect.GetEndPoint();
    return out;
}

wostream& operator<< (wostream& out, MicroMeterCircle const& circle)
{
    out << OPEN_BRACKET
        << circle.GetPos()
        << ID_SEPARATOR
        << circle.GetRadius()
        << CLOSE_BRACKET;
    return out;
}

wostream& operator<< (wostream& out, Color const& color)
{
    out << setprecision(6)
        << L"RGB"
        << OPEN_BRACKET
        << color.r
        << ID_SEPARATOR
        << color.g
        << ID_SEPARATOR
        << color.b
        << CLOSE_BRACKET;
    return out;
}

MicroMeter ScrReadMicroMeter(Script& script)
{
    float const fValue = Cast2Float(script.ScrReadFloat());
    if (fabs(fValue) > MAX_MICRO_METER.GetValue())
        throw ScriptErrorHandler::ScriptException(777, L"MicroMeter value too big");
    return MicroMeter(fValue);
}

MicroMeterPnt ScrReadMicroMeterPnt(Script& script)
{
    script.ScrReadSpecial(OPEN_BRACKET);
    MicroMeter const x(ScrReadMicroMeter(script));
    script.ScrReadSpecial(SEPARATOR);
    MicroMeter const y(ScrReadMicroMeter(script));
    script.ScrReadSpecial(CLOSE_BRACKET);
    return MicroMeterPnt(x, y);
}

MicroMeterRect ScrReadMicroMeterRect(Script& script)
{
    MicroMeterPnt const umPntStart(ScrReadMicroMeterPnt(script));
    MicroMeterPnt const umPntEnd  (ScrReadMicroMeterPnt(script));
    return MicroMeterRect(umPntStart, umPntEnd);
}

MicroMeterCircle ScrReadMicroMeterCircle(Script& script)
{
    script.ScrReadSpecial(OPEN_BRACKET);
    MicroMeterPnt umCenter { ScrReadMicroMeterPnt(script) };
    script.ScrReadSpecial(ID_SEPARATOR);
    MicroMeter    umRadius { ScrReadMicroMeter(script) };
    script.ScrReadSpecial(CLOSE_BRACKET);
    return MicroMeterCircle(umCenter, umRadius);
}

fMicroSecs ScrReadfMicroSecs(Script& script)
{
    return fMicroSecs(Cast2Float(script.ScrReadFloat()));
}

tBoolOp ScrReadBoolOp(Script & script)
{
    unsigned long ulBoolOp(script.ScrReadUlong());
    return static_cast<tBoolOp>(ulBoolOp);
}

Color ScrReadColor(Script& script)
{
    script.ScrReadString(L"RGB");
    script.ScrReadSpecial(OPEN_BRACKET);
    float fRed   { Cast2Float(script.ScrReadFloat()) };
    script.ScrReadSpecial(ID_SEPARATOR);
    float fGreen { Cast2Float(script.ScrReadFloat()) };
    script.ScrReadSpecial(ID_SEPARATOR);
    float fBlue  { Cast2Float(script.ScrReadFloat()) };
    script.ScrReadSpecial(CLOSE_BRACKET);
    return Color(fRed, fGreen, fBlue);
}

class WrapOpenTraceFile : public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
		//OpenTraceFile(script.ScrReadString());
    }
};

void DefineUtilityWrapperFunctions()
{
    SymbolTable::ScrDefConst(L"OpenTraceFile", new WrapOpenTraceFile);

    SymbolTable::ScrDefConst(L"tBoolOp::opTrue",     static_cast<unsigned long>(tBoolOp::opTrue    ));
    SymbolTable::ScrDefConst(L"tBoolOp::opFalse",    static_cast<unsigned long>(tBoolOp::opFalse   ));
    SymbolTable::ScrDefConst(L"tBoolOp::opToggle",   static_cast<unsigned long>(tBoolOp::opToggle  ));
    SymbolTable::ScrDefConst(L"tBoolOp::opNoChange", static_cast<unsigned long>(tBoolOp::opNoChange));
}
