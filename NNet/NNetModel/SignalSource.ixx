// SignalSource.ixx
//
// NNetModel

module;

#include <iostream>

export module NNetModel:SignalSource;

import Observable;
import Types;
import DrawContext;

using std::wostream;

export class SignalSource : public Observable
{
public:
    virtual ~SignalSource() = default;

    virtual void Dump()                                const = 0;
    virtual mV   GetSignalValue()                      const = 0;
    virtual void WriteInfo(wostream &)                 const = 0;
    virtual void Draw(DrawContext const &, bool const) const = 0;
};
