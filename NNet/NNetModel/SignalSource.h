// SignalSource.h : 
//
// NNetModel

#pragma once

#include <ostream>
#include "MoreTypes.h"
#include "observable.h"

using std::wostream;

class UPNobList;
class DrawContext;

class SignalSource : public Observable
{
public:
    virtual void Dump()                                const = 0;
    virtual mV   GetSignalValue()                      const = 0;
    virtual void WriteData(wostream &)                 const = 0;
    virtual void Draw(DrawContext const &, bool const) const = 0;
    virtual bool Includes(MicroMeterPnt const)         const = 0;

    virtual void Recalc(UPNobList const &) = 0;
};
