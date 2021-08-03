// Signal.h : 
//
// NNetModel

#pragma once

#include <vector>
#include <math.h>    
#include "Observable.h"
#include "ObserverInterface.h"
#include "MoreTypes.h"
#include "NNetParameters.h"

using std::vector;

class Signal;
class DrawContext;
class NNetModelReaderInterface;

using SignalFunc = function<void(Signal const &)>;
using SignalCrit = function<bool(Signal const &)>;

class Signal : public ObserverInterface  // observes signal source 
{
public:

    Signal
    (
        NNetModelReaderInterface const &,
        Observable                     &,
        MicroMeterCircle         const & 
   );

    virtual ~Signal();

    bool const operator==(Signal const & rhs) const
    {
        return m_circle == rhs.m_circle;
    }

    fMicroSecs const GetStartTime() const { return m_timeStart; }
    float      const GetDataPoint   (fMicroSecs const) const;
    fMicroSecs const FindNextMaximum(fMicroSecs const) const;

    void  Notify(bool const);
    void  Draw(DrawContext const &) const;
    float GetSignalValue() const;
    void  WriteSignalData(wostream &) const;;

    bool  Includes(MicroMeterPnt const pos) const { return m_circle.Includes(pos); }

    void  Move(MicroMeterPnt const & umDelta) { m_circle += umDelta; }
    void  Size(float           const   factor ) { m_circle *= factor; }

    MicroMeterPnt    const & GetCenter() const { return m_circle.GetPos(); }
    MicroMeterCircle const & GetCircle() const { return m_circle; }

    void Set2Null() { m_circle.Set2Null(); }

    void Check() const {};
    void Dump() const {};
    void CheckSignal();

private:

    NNetModelReaderInterface const & m_nmri;
    Observable                     & m_observable;

    MicroMeterCircle m_circle    { MicroMeterCircle::NULL_VAL() };
    fMicroSecs       m_timeStart { 0._MicroSecs };
    vector <float>   m_data      { };

    int        const time2index(fMicroSecs const) const;
    fMicroSecs const index2time(int        const) const;
};
