// MicroSensor.ixx
//
// NNetModel

module;

#include <iostream>

export module NNetModel:MicroSensor;

import Types;
import DrawContext;
import :SignalSource;
import :NobId;

using std::wostream;

class Nob;
class NNetModelIo;

export class MicroSensor : public SignalSource
{
public:

    MicroSensor(Nob * const);

    virtual ~MicroSensor() = default;

    void Dump()                               const final;
    mV   GetSignalValue()                     const final;
    void Draw(DrawContext const&, bool const) const final;
    MicroMeterPnt GetPosition()               const final;

    SignalSource::Type SignalSourceType() const final 
    { 
        return SignalSource::Type::microSensor; 
    };

    NobId      GetNobId() const;
    Nob const* GetNob  () const { return m_pNob; }

private:

    Nob* m_pNob;
};

export MicroSensor const* Cast2MicroSensor(SignalSource const* pSource)
{
    return pSource && pSource->IsMicroSensor()
        ? static_cast<MicroSensor const*>(pSource)
        : nullptr;
}

export MicroSensor* Cast2MicroSensor(SignalSource* pSource)
{
    return pSource && pSource->IsMicroSensor()
        ? static_cast<MicroSensor*>(pSource)
        : nullptr;
}
