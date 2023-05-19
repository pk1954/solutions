// SignalSource.ixx
//
// Signals

export module Signals:SignalSource;

import Observable;
import Types;
import DrawContext;

export class SignalSource : public Observable
{
public:
    virtual ~SignalSource() = default;

    virtual void Dump()           const = 0;
    virtual mV   GetSignalValue() const = 0;

    virtual void Draw(DrawContext const&, bool const) const {};
    virtual MicroMeterPnt GetPosition() const { return MicroMeterPnt::NULL_VAL(); }
};
