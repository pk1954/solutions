// NNetSignalSource.ixx
//
// NNetModel

export module NNetModel:NNetSignalSource;

import std;
import Observable;
import Types;
import DrawContext;
import Signals;

using std::wostream;

export class NNetSignalSource : public SignalSource
{
public:
    enum class Type
    {
        generator,
        macroSensor,
        microSensor
    };

    virtual ~NNetSignalSource() = default;

    virtual void Dump()             const = 0;
    virtual mV   GetSignalValue()   const = 0;
    virtual Type SignalSourceType() const = 0;

    virtual void Draw(DrawContext const&, bool const) const {};
    virtual MicroMeterPnt GetPosition() const { return MicroMeterPnt::NULL_VAL(); }

    bool IsGenerator  () const { return SignalSourceType() == Type::generator; }
    bool IsSensor     () const { return SignalSourceType() == Type::macroSensor; }
    bool IsMicroSensor() const { return SignalSourceType() == Type::microSensor; }
};
