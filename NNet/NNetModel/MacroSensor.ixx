// MacroSensor.ixx
//
// NNetModel

module;

#include <vector>
#include <iostream>

export module NNetModel:MacroSensor;

import Types;
import DrawContext;
import :NNetParameters;
import :Sensor;
import :UPNobList;
import :Pipe;

using std::vector;
using std::wostream;

export class MacroSensor : public Sensor
{
public:
    MacroSensor(MicroMeterCircle const &, UPNobList const &);

    void Dump()                                      const final;
    mV   GetSignalValue()                            const final;
    void WriteInfo(wostream &)                       const final;
    void DrawSigSrc(DrawContext const &, bool const) const final;
    bool IsConnected()                               const final { return true; }

    void RotateSensor(MicroMeterPnt const&, Radian const) final;

    bool Includes(MicroMeterPnt const &p) const { return m_circle.Includes(p); }

    MicroMeterPnt GetPosition() const final { return GetCenter(); }

    void  DrawDataPoints(DrawContext  const &) const;
    float GetDistFactor(MicroMeterPnt const &) const;

    MicroMeterPnt    const & GetCenter()       const { return m_circle.GetPos(); }
    MicroMeter               GetRadius()       const { return m_circle.GetRadius(); }
    MicroMeterCircle const & GetCircle()       const { return m_circle; }
    size_t                   GetNrOfElements() const { return m_dataPoints.size(); }

    void SetSensorPos (UPNobList const &, MicroMeterPnt const &);
    void MoveSensor   (UPNobList const &, MicroMeterPnt const &);
    void SetSensorSize(UPNobList const &, MicroMeter    const);
    void SizeSensor   (UPNobList const &, float         const);
    void Recalc       (UPNobList const &);

    Sensor::Type SensorType() const final { return Sensor::Type::macroSensor; }

private:

    struct SigDataPoint
    {
        Pipe          const * m_pPipe;
        Pipe::SegNr   const   m_segNr;
        MicroMeterPnt const   m_umPos;
        float         const   m_fFactor;

        mV GetSignalValue() const
        {
            return m_pPipe->GetVoltage(m_segNr) * m_fFactor;
        }

        MicroMeterCircle dataPointCircle() const
        {
            return MicroMeterCircle(m_umPos, PIPE_WIDTH * 0.5f);
        }
    };

    MicroMeterCircle     m_circle{ MicroMeterCircle::NULL_VAL() };
    vector<SigDataPoint> m_dataPoints{ };

    void add2list(Pipe const &);
};

export MacroSensor const* Cast2MacroSensor(Sensor const* pSensor)
{
    return pSensor && pSensor->IsMacroSensor()
        ? static_cast<MacroSensor const*>(pSensor)
        : nullptr;
}
