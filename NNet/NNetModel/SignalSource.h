// SignalSource.h : 
//
// NNetModel

#pragma once

#include "observable.h"

class UPNobList;

class SignalSource : public Observable
{
public:
    virtual void  Dump()                                const = 0;
    virtual float GetSignalValue()                      const = 0;
    virtual void  WriteData(wostream &)                 const = 0;
    virtual void  Draw(DrawContext const &, bool const) const = 0;
    virtual bool  Includes(MicroMeterPnt const)         const = 0;

    virtual void  Recalc(UPNobList const &) = 0;
};
