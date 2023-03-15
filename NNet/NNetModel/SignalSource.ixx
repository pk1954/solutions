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
    enum class Type
    {
        generator,
        sensor
    };

    virtual ~SignalSource() = default;

    virtual void Dump()                                      const = 0;
    virtual mV   GetSignalValue()                            const = 0;
    virtual void WriteInfo(wostream &)                       const = 0;
    virtual void DrawSigSrc(DrawContext const &, bool const) const = 0;
    virtual Type SignalSourceType()                          const = 0;
    virtual bool IsConnected()                               const = 0;

    bool IsGenerator() const { return SignalSourceType() == Type::generator; }
    bool IsSensor   () const { return SignalSourceType() == Type::sensor; }
};
