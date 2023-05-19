// MicroSensor.ixx
//
// NNetModel

module;

#include <iostream>

export module NNetModel:MicroSensor;

import Types;
import DrawContext;
import :NNetSignalSource;
import :NobId;

using std::wostream;

class Nob;
class NNetModelIo;

export class MicroSensor : public NNetSignalSource
{
public:

    MicroSensor(Nob * const);

    virtual ~MicroSensor() = default;

    void Dump()                               const final;
    mV   GetSignalValue()                     const final;
    void Draw(DrawContext const&, bool const) const final;
    MicroMeterPnt GetPosition()               const final;

    NNetSignalSource::Type SignalSourceType() const final
    { 
        return NNetSignalSource::Type::microSensor;
    };

    NobId      GetNobId() const;
    Nob const* GetNob  () const { return m_pNob; }

private:

    Nob* m_pNob;
};

export MicroSensor const* Cast2MicroSensor(NNetSignalSource const* pSource)
{
    return pSource && pSource->IsMicroSensor()
        ? static_cast<MicroSensor const*>(pSource)
        : nullptr;
}

export MicroSensor* Cast2MicroSensor(NNetSignalSource* pSource)
{
    return pSource && pSource->IsMicroSensor()
        ? static_cast<MicroSensor*>(pSource)
        : nullptr;
}
