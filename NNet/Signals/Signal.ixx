// Signal.ixx
//
// Signals

module;

#include <iostream>
#include <vector>
#include <math.h>    
#include <memory>    

export module Signals:Signal;

import Observable;
import ObserverInterface;
import Types;
import DrawContext;
import SimulationTime;
import :SignalParameters;

using std::vector;
using std::wostream;
using std::unique_ptr;

class NNetModelIO;

export using SIG_INDEX = long;
export using SIMU_TIME = fMicroSecs;

export class Signal : public ObserverInterface  // observes signal source 
{
public:

    static void Initialize(Observable * const pObs)
    {
        m_pDynModelObservable = pObs;
    }

    Signal();

    virtual ~Signal();

    // TODO: Rule of 5

    //bool operator==(Signal const &) const;
    // TODO
    void Reset();

    mV        GetDataPoint   (SignalParameters const &, SIMU_TIME const)  const;
    SIMU_TIME FindNextMaximum(SignalParameters const &, SIMU_TIME const)  const;

    void WriteSignalData(wostream &) const;

    SIMU_TIME GetStartTime() const { return m_timeStart; }
    void      SetStartTime(SIMU_TIME const t) { m_timeStart = t; }

    void Reserve(size_t const);
    void Add2Signal(mV const);

    void Check()       const { /**/ };
    void Dump()        const;
    void CheckSignal() const;

    fMicroSecs GetResolution() const { return m_usResolution; }

    inline static int const SIGSRC_CIRCLE { 101 };
    inline static int const SIGSRC_NOB    { 103 };

private:

    inline static SIG_INDEX INVALID_SIG_INDEX{ -1 };

    inline static Observable * m_pDynModelObservable;

    SIMU_TIME    m_timeStart { SimulationTime::Get() };
    vector<mV>   m_data;
    fMicroSecs   m_usResolution { 100._MicroSecs };

    SIG_INDEX time2index(SignalParameters const &, SIMU_TIME) const;
    SIMU_TIME index2time(SignalParameters const &, SIG_INDEX const) const;
};
