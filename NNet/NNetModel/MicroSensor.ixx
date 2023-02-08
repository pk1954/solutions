// MicroSensor.ixx
//
// NNetModel

module;

#include <iostream>

export module NNetModel:MicroSensor;

import Types;
import DrawContext;
import :Sensor;
import :NobId;
import :Nob;

using std::wostream;

export class MicroSensor : public Sensor
{
public:

    MicroSensor(Nob * const);

    virtual ~MicroSensor() = default;

    void Dump()                               const final;
    mV   GetSignalValue()                     const final;
    void WriteInfo(wostream&)                 const final;
    void Draw(DrawContext const&, bool const) const final;

    MicroMeterPnt GetPosition() const final { return m_pNob->GetPos(); }

    NobId GetNobId() const { return m_pNob->GetId(); }

    Sensor::Type SensorType() const final { return Sensor::Type::microSensor; }

    void RotateSensor(MicroMeterPnt const&, Radian const) final { /* nob does rotation */ };

private:

    Nob* m_pNob;
};

export MicroSensor const* Cast2MicroSensor(Sensor const* pSensor)
{
    return pSensor && pSensor->IsMicroSensor()
        ? static_cast<MicroSensor const*>(pSensor)
        : nullptr;
}
