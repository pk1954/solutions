// Signal.h : 
//
// NNetModel

#pragma once

#include <vector>
#include <math.h>    
#include "observable.h"
#include "observerInterface.h"
#include "MoreTypes.h"
#include "NNetParameters.h"
#include "Pipe.h"

using std::vector;
using std::unique_ptr;

class Signal;
class BaseKnot;
class DrawContext;
class NNetModelReaderInterface;

class Signal : public ObserverInterface  // observes signal source 
{
public:

    Signal
    (
        NNetModelReaderInterface const &,
        Observable                     &,
        MicroMeterCircle         const & 
    );

    ~Signal() override;

    bool operator==(Signal const & rhs) const { return m_circle == rhs.m_circle; }

    void       SetSensorPos (MicroMeterPnt const &);
    void       MoveSensor   (MicroMeterPnt const &);
    void       SizeSensor   (float         const);
    void       SetSensorSize(MicroMeter    const);
    void       RotateSensor (MicroMeterPnt const &, Radian const);
    void       Recalc();
    void       Reset();

    float      GetDistFactor(MicroMeterPnt const &)  const;
    float      GetSignalValue()                      const;
    float      GetDataPoint   (fMicroSecs const)     const;
    fMicroSecs FindNextMaximum(fMicroSecs const)     const;
    void       Draw(DrawContext const &, bool const) const;
    void       DrawDataPoints(DrawContext const &)   const;
    void       WriteSignalData(wostream &)           const;

    fMicroSecs               GetStartTime   () const { return m_timeStart; }
    MicroMeterPnt    const & GetCenter      () const { return m_circle.GetPos(); }
    MicroMeter               GetRadius      () const { return m_circle.GetRadius(); }
    MicroMeterCircle const & GetCircle      () const { return m_circle; }
    size_t                   GetNrOfElements() const { return m_dataPoints.size(); }

    bool Includes(MicroMeterPnt const pos) const { return m_circle.Includes(pos); }

    void Set2Null() { m_circle.Set2Null(); }

    void Notify(bool const) final;

    void Check() const { /**/  };
    void Dump()  const;
    void CheckSignal() const;

private:

    NNetModelReaderInterface const & m_nmri;
    Observable                     & m_observable;

    MicroMeterCircle m_circle    { MicroMeterCircle::NULL_VAL() };
    float            m_fDsBorder { };
    fMicroSecs       m_timeStart { 0._MicroSecs };
    vector<float>    m_fTimeLine { };
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
    vector<SigDataPoint> m_dataPoints { };

    SigDataPoint const * findDataPoint(MicroMeterPnt const &) const;
    int                  time2index   (fMicroSecs    const  ) const;
    fMicroSecs           index2time   (int           const  ) const;
    void                 add2list     (Pipe          const &);
};
