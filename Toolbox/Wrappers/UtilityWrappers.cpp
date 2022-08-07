// UtilityWrappers.cpp
//
// Utilities\Wrappers

#include <assert.h>
#include "Script.h"
#include "Symtab.h"
#include "UtilityWrappers.h"

import BoolOp;
import Trace;
import PixelTypes;

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
    DEF_FUNC(OpenTraceFile);

    DEF_ULONG_CONST(tBoolOp::opTrue);
    DEF_ULONG_CONST(tBoolOp::opFalse);
    DEF_ULONG_CONST(tBoolOp::opToggle);
    DEF_ULONG_CONST(tBoolOp::opNoChange);
}
