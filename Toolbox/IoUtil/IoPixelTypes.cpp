// IoPixelTypes.cpp
//
// Toolbox\IoUtil

module;

#include <compare>

module IoUtil:IoPixelTypes;

import Types;
import Script;
import SaveCast;

PIXEL ScrReadPixel(Script& script)
{
    return PIXEL(script.ScrReadLong());
}

fPixel ScrReadfPixel(Script& script)
{
    return fPixel(Cast2Float(script.ScrReadFloat()));
}

PixelPoint ScrReadPixelPoint(Script& script)
{
    PIXEL const x(ScrReadPixel(script));
    PIXEL const y(ScrReadPixel(script));
    return PixelPoint(x, y);
}

fPixelPoint ScrReadfPixelPoint(Script& script)
{
    script.ScrReadSpecial(L'(');
    fPixel const x(ScrReadfPixel(script));
    script.ScrReadSpecial(L'/');
    fPixel const y(ScrReadfPixel(script));
    script.ScrReadSpecial(L')');
    return fPixelPoint(x, y);
}

PixelRectSize ScrReadPixelRectSize(Script& script)
{
    PixelPoint const pixPnt(ScrReadPixelPoint(script));
    return PixelRectSize(pixPnt.GetX(), pixPnt.GetY());
}

PixelRect ScrReadPixelRect(Script& script)
{
    PixelPoint    const pixPos(ScrReadPixelPoint(script));
    PixelRectSize const pixSize(ScrReadPixelRectSize(script));
    return PixelRect(pixPos, pixSize);
}
