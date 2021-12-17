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
#include "SignalSource.h"
#include "Pipe.h"

using std::vector;
using std::unique_ptr;

class Signal;
class BaseKnot;
class SignalSource;
class DrawContext;
class NNetModelReaderInterface;

class Signal : public ObserverInterface  // observes signal source 
{
public:

    using Crit = function<bool(Signal const &)>;

    Signal
    (
        NNetModelReaderInterface const &,
        Observable                     &,
        MicroMeterCircle         const & 
    );

    ~Signal() override;

    bool operator==(Signal const & rhs) const
    {
        return m_circle == rhs.m_circle;
    }

    void       Recalc();

    fMicroSecs GetStartTime() const { return m_timeStart; }

    float      GetDataPoint   (fMicroSecs const)     const;
    fMicroSecs FindNextMaximum(fMicroSecs const)     const;
    void       Draw(DrawContext const &, bool const) const;
    float      GetSignalValue()                      const;
    void       WriteSignalData(wostream &)           const;

    void  Notify(bool const) final;

    bool  Includes(MicroMeterPnt const pos) const { return m_circle.Includes(pos); }

    void  SetSensorPos (MicroMeterPnt const & umPos  ) { m_circle.SetPos(umPos); }
    void  MoveSensor   (MicroMeterPnt const & umDelta) { m_circle += umDelta; }
    void  SizeSensor   (float         const   factor ) { SetSensorSize(GetRadius() * factor); }
    void  SetSensorSize(MicroMeter    const   umSize ) 
    { 
        m_circle.SetRadius(umSize); 
        m_fDsBorder = umSize.GetValue() * umSize.GetValue();
    }

    MicroMeterPnt    const & GetCenter() const { return m_circle.GetPos(); }
    MicroMeter               GetRadius() const { return m_circle.GetRadius(); }
    MicroMeterCircle const & GetCircle() const { return m_circle; }

    size_t GetNrOfBaseKnotElements() const { return m_baseKnotElements.size(); }
    size_t GetNrOfPipeSegElements () const { return m_pipeSegElements .size(); }

    void Set2Null() { m_circle.Set2Null(); }

    void Check() const {};
    void Dump()  const;
    void CheckSignal() const;

private:

    NNetModelReaderInterface const & m_nmri;
    Observable                     & m_observable;

    MicroMeterCircle m_circle    { MicroMeterCircle::NULL_VAL() };
    float            m_fDsBorder { };
    fMicroSecs       m_timeStart { 0._MicroSecs };
    vector<float>    m_fTimeLine { };
    struct BaseKnotElem
    {
        BaseKnot const * m_pBaseKnot;
        float    const   m_fFactor;
    };
    vector<BaseKnotElem> m_baseKnotElements { };
    struct SegmentElem
    {
        Pipe        const * m_pPipe;
        Pipe::SegNr const   m_segNr;
        float       const   m_fFactor;
    };
    vector<SegmentElem> m_pipeSegElements { };

    int        time2index(fMicroSecs const) const;
    fMicroSecs index2time(int        const) const;
    void       add2list(BaseKnot const &);
    void       add2list(Pipe const &);
};
