// SignalSource.h : 
//
// NNetModel

#pragma once

#include <iostream>
#include "UPNobList.h"

import Observable;
import Types;
import DrawContext;

using std::wostream;

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
