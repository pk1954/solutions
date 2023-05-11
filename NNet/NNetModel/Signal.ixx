// Signal.ixx
//
// NNetModel

module;

#include <iostream>
#include <vector>
#include <math.h>    
#include <memory>    

export module NNetModel:Signal;

import Observable;
import ObserverInterface;
import Types;
import DrawContext;
import SimulationTime;
import :NNetParameters;
import :SignalSource;

using std::vector;
using std::wostream;
using std::unique_ptr;

class NNetModelIO;

export using SIG_INDEX = long;
export using SIMU_TIME = fMicroSecs;

export class Signal : public ObserverInterface  // observes signal source 
{
public:

    Signal(Observable &, SignalSource &);

    // TODO: Rule of 5

    ~Signal() final;

    //bool operator==(Signal const &) const;
    // TODO
    void Reset();

    mV        GetDataPoint   (NNetParameters const &, SIMU_TIME const)  const;
    SIMU_TIME FindNextMaximum(NNetParameters const &, SIMU_TIME const)  const;

    void WriteSignalData(wostream &) const;

    //bool Includes(MicroMeterPnt const) const;

    SIMU_TIME GetStartTime() const { return m_timeStart; }
    void      SetStartTime(SIMU_TIME const t) { m_timeStart = t; }

    void Reserve(size_t const);
    void Add(mV const);

    void Notify(bool const) final;

    void Check()       const { /**/ };
    void Dump()        const;
    void CheckSignal() const;

    int GetSigSrcType() const { return m_iSourceType; }

    SignalSource const * GetSignalSource() const { return &m_sigSource; }

    inline static int const SIGSRC_CIRCLE { 101 };
    inline static int const SIGSRC_NOB    { 103 };

private:

    inline static SIG_INDEX INVALID_SIG_INDEX{ -1 };

    Observable   & m_dynModelObservable;
    SignalSource & m_sigSource;
    SIMU_TIME      m_timeStart { SimulationTime::Get() };
    vector<mV>     m_data;
    int            m_iSourceType;

    SIG_INDEX time2index(NNetParameters const &, SIMU_TIME) const;
    SIMU_TIME index2time(NNetParameters const &, SIG_INDEX const) const;
};
