// Signal.ixx
//
// Signals

export module Signals:Signal;

import std;
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

    mV       GetDataPoint   (SignalParameters const &, SimuTime const)  const;
    SimuTime FindNextMaximum(SignalParameters const &, SimuTime const)  const;

    void WriteSignalData(wostream &) const;

    SimuTime GetStartTime() const { return m_timeStart; }
    void     SetStartTime(SimuTime const t) { m_timeStart = t; }

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

    SimuTime   m_timeStart { SimulationTime::Get() };
    vector<mV> m_data;
    fMicroSecs m_usResolution { 100._MicroSecs };

    SIG_INDEX time2index(SignalParameters const &, SimuTime) const;
    SimuTime  index2time(SignalParameters const &, SIG_INDEX const) const;
};
