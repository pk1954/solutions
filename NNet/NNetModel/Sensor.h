// Sensor.h : 
//
// NNetModel

#pragma once

#include <vector>
#include "Pipe.h"
#include "SignalSource.h"

class DrawContext;

class Sensor : public SignalSource
{
public:
    Sensor(MicroMeterCircle const &, UPNobList const &);

    void  Dump()                                const final;
    float GetSignalValue()                      const final;
    void  WriteData(wostream &)                 const final;
    void  Draw(DrawContext const &, bool const) const final;

    void  DrawDataPoints(DrawContext   const &) const;
    float GetDistFactor (MicroMeterPnt const &) const;
    bool  Includes      (MicroMeterPnt const p) const { return m_circle.Includes(p); }
   
    MicroMeterPnt    const & GetCenter      () const { return m_circle.GetPos(); }
    MicroMeter               GetRadius      () const { return m_circle.GetRadius(); }
    MicroMeterCircle const & GetCircle      () const { return m_circle; }
    size_t                   GetNrOfElements() const { return m_dataPoints.size(); }

    void SetSensorPos (UPNobList const &, MicroMeterPnt const &);
    void MoveSensor   (UPNobList const &, MicroMeterPnt const &);
    void SetSensorSize(UPNobList const &, MicroMeter    const);
    void SizeSensor   (UPNobList const &, float         const);
    void RotateSensor (MicroMeterPnt const &, Radian const);

private:

    struct SigDataPoint
    {
        Pipe          const * m_pPipe;
        Pipe::SegNr   const   m_segNr;
        MicroMeterPnt const   m_umPos;
        float         const   m_fFactor;

        float GetSignalValue() const
        {
            return m_pPipe->GetVoltage(m_segNr).GetValue() * m_fFactor;
        }

        MicroMeterCircle dataPointCircle() const
        {
            return MicroMeterCircle(m_umPos, PIPE_WIDTH * 0.5f);
        }
    };

    MicroMeterCircle     m_circle     { MicroMeterCircle::NULL_VAL() };
    vector<SigDataPoint> m_dataPoints { };

    void Recalc(UPNobList const &) final;

    void add2list(Pipe const &);

    SigDataPoint const * findDataPoint(MicroMeterPnt const &) const;
};
