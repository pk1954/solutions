// Sensor.ixx
//
// NNetModel

export module NNetModel:Sensor;

import std;
import Types;
import DrawContext;
import :NNetParameters;
import :NNetSignalSource;
import :UPNobList;
import :Pipe;

using std::vector;
using std::wostream;

class NNetModelIO;

export class Sensor : public NNetSignalSource
{
public:
    Sensor(MicroMeterCircle const &, UPNobList const &);

    NNetSignalSource::Type SignalSourceType() const final { return NNetSignalSource::Type::macroSensor; };

    void Dump()           const final;
    mV   GetSignalValue() const final;

    void WriteInfo(wostream&) const;

    void RotateSensor(MicroMeterPnt const&, Radian const);

    bool Includes(MicroMeterPnt const &p) const { return m_circle.Includes(p); }

    void  Draw(DrawContext const &, bool const) const final;
    MicroMeterPnt GetPosition()                 const final { return GetCenter(); }

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

export Sensor const* Cast2Sensor(NNetSignalSource const* pSource)
{
    return pSource && pSource->IsSensor()
        ? static_cast<Sensor const*>(pSource)
        : nullptr;
}

export Sensor* Cast2Sensor(NNetSignalSource* pSource)
{
    return pSource && pSource->IsSensor()
        ? static_cast<Sensor*>(pSource)
        : nullptr;
}
