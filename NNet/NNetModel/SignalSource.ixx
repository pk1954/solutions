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
        macroSensor,
        microSensor
    };

    virtual ~SignalSource() = default;

    virtual void Dump()             const = 0;
    virtual mV   GetSignalValue()   const = 0;
    virtual Type SignalSourceType() const = 0;

    virtual void Draw(DrawContext const&, bool const) const {};
    virtual MicroMeterPnt GetPosition() const { return MicroMeterPnt::NULL_VAL(); }

    bool IsGenerator  () const { return SignalSourceType() == Type::generator; }
    bool IsSensor     () const { return SignalSourceType() == Type::macroSensor; }
    bool IsMicroSensor() const { return SignalSourceType() == Type::microSensor; }
};
