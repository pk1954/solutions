// SignalSource.h : 
//
// NNetModel

#pragma once

#include <ostream>
#include "MoreTypes.h"

import Observable;

using std::wostream;

class UPNobList;
class DrawContext;

class SignalSource : public Observable
{
public:
    virtual ~SignalSource() = default;

    virtual void Dump()                                const = 0;
    virtual mV   GetSignalValue()                      const = 0;
    virtual void WriteInfo(wostream &)                 const = 0;
    virtual void Draw(DrawContext const &, bool const) const = 0;
    virtual bool Includes(MicroMeterPnt const)         const = 0;

    virtual void Recalc(UPNobList const &) = 0;
};
