// IoUtil.cpp
//
// Toolbox\IoUtil

module;

#include <cassert>
#include <compare>
#include <iostream>
#include <iomanip>

module IoUtil;

import SaveCast;
import BoolOp;
import Trace;
import Types;
import Symtab;
import Script;
import :IoConstants;

using std::wostream;
using std::setprecision;

wostream& operator<< (wostream& out, MicroMeterPnt const& pnt)
{
    out << L' '
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

MicroMeterCircle ScrReadMicroMeterCircle(Script& script)
{
    script.ScrReadSpecial(OPEN_BRACKET);
    MicroMeterPnt umCenter { ScrReadMicroMeterPnt(script) };
    script.ScrReadSpecial(ID_SEPARATOR);
    MicroMeter      umRadius { ScrReadMicroMeter(script) };
    script.ScrReadSpecial(CLOSE_BRACKET);
    return MicroMeterCircle(umCenter, umRadius);
}

PIXEL ScrReadPixel(Script & script)
{
    return PIXEL(script.ScrReadLong());
}

fPixel ScrReadfPixel(Script & script)
{
    return fPixel(Cast2Float(script.ScrReadFloat()));
}

PixelPoint ScrReadPixelPoint(Script & script)
{
    PIXEL const x(ScrReadPixel(script));
    PIXEL const y(ScrReadPixel(script));
    return PixelPoint(x, y);
}

fPixelPoint ScrReadfPixelPoint(Script & script)
{
    script.ScrReadSpecial(L'(');
    fPixel const x(ScrReadfPixel(script));
    script.ScrReadSpecial(L'/');
    fPixel const y(ScrReadfPixel(script));
    script.ScrReadSpecial(L')');
    return fPixelPoint(x, y);
}

PixelRectSize ScrReadPixelRectSize(Script & script)
{
    PixelPoint const pixPnt(ScrReadPixelPoint(script));
    return PixelRectSize(pixPnt.GetX(), pixPnt.GetY());
}

PixelRect ScrReadPixelRect(Script & script)
{
    PixelPoint    const pixPos (ScrReadPixelPoint   (script));
    PixelRectSize const pixSize(ScrReadPixelRectSize(script));
    return PixelRect(pixPos, pixSize);
}

tBoolOp ScrReadBoolOp(Script & script)
{
    unsigned long ulBoolOp(script.ScrReadUlong());
    return static_cast<tBoolOp>(ulBoolOp);
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
