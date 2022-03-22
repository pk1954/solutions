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

class Param;
class Signal;
class BaseKnot;
class UPNobList;
class MeanFilter;
class DrawContext;
class NNetModelReaderInterface;

class Signal : public ObserverInterface  // observes signal source 
{
public:

    Signal
    (
        Observable             &,
        UPNobList        const &,
        MicroMeterCircle const & 
    );

    ~Signal() override;

    bool operator==(Signal const & rhs) const { return m_circle == rhs.m_circle; }

    void       SetSensorPos (UPNobList const &, MicroMeterPnt const &);
    void       MoveSensor   (UPNobList const &, MicroMeterPnt const &);
    void       SetSensorSize(UPNobList const &, MicroMeter    const);
    void       SizeSensor   (UPNobList const &, float         const);
    void       RotateSensor (MicroMeterPnt const &, Radian const);
    void       RecalcFilter (Param const &) const;
    void       Recalc       (UPNobList const &);
    void       Reset        (fMicroSecs const);

    float      GetFilteredDataPoint(Param const &, fMicroSecs const) const;
    float      GetRawDataPoint     (Param const &, fMicroSecs const) const;
    fMicroSecs FindNextMaximum     (Param const &, fMicroSecs const) const;
    float      GetDistFactor       (MicroMeterPnt const &)           const;
    float      GetSignalValue      ()                                const;
    void       Draw                (DrawContext const &, bool const) const;
    void       DrawDataPoints      (DrawContext const &)             const;
    void       WriteSignalData     (wostream &)                      const;

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

    Observable           & m_observable;
    MicroMeterCircle       m_circle    { MicroMeterCircle::NULL_VAL() };
    float                  m_fDsBorder { };
    fMicroSecs             m_timeStart { 0._MicroSecs };
    unique_ptr<MeanFilter> m_upMeanFilter;
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

    int        time2index(Param const &, fMicroSecs const  ) const;
    fMicroSecs index2time(Param const &, int        const  ) const;
    void       add2list  (Pipe const &);
};
