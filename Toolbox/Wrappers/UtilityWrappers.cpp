// UtilityWrappers.cpp
//
// Toolbox\Wrappers

module;

#include <cassert>
#include <compare>

module UtilityWrappers;

import SaveCast;
import BoolOp;
import Trace;
import Types;
import Symtab;
import Script;

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
