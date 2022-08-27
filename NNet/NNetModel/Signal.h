// Signal.h : 
//
// NNetModel

#pragma once

#include <vector>
#include <math.h>    
#include <memory>    
#include "BaseKnot.h"
#include "UPNobList.h"
#include "SignalSource.h"

import ObserverInterface;
import BasicTypes;
import MoreTypes;
import DrawContext;
import SimulationTime;
import NNetParameters;

using std::vector;
using std::unique_ptr;

using SIG_INDEX = long;
using SIMU_TIME = fMicroSecs;

class Signal : public ObserverInterface  // observes signal source 
{
public:

    Signal(Observable &, SignalSource &);

    ~Signal() override;

    void Reset();

    mV        GetDataPoint   (Param const &, SIMU_TIME const) const;
    SIMU_TIME FindNextMaximum(Param const &, SIMU_TIME const) const;
    void      Draw           (DrawContext const &, bool const) const;
    void      WriteSignalInfo(wostream &)                      const;
    void      WriteSignalData(wostream &)                      const;

    bool Includes(MicroMeterPnt const) const;

    SIMU_TIME GetStartTime() const { return m_timeStart; }
    void      SetStartTime(SIMU_TIME const t) { m_timeStart = t; }

    void Reserve(size_t const);
    void Add(mV const);

    void Notify(bool const) final;

    void Check()       const { /**/  };
    void Dump()        const;
    void CheckSignal() const;

    int GetSigSrcType() const { return m_iSourceType; }

    SignalSource const * GetSignalSource() const { return & m_sigSource; }

    inline static int const SIGSRC_CIRCLE    { 101 };
    inline static int const SIGSRC_GENERATOR { 102 };

private:

    inline static SIG_INDEX INVALID_SIG_INDEX { -1 };

    Observable   & m_dynModelObservable;
    SignalSource & m_sigSource;
    SIMU_TIME      m_timeStart { SimulationTime::Get() };
    vector<mV>     m_data;
    int            m_iSourceType;

    SIG_INDEX  time2index(Param const &, SIMU_TIME) const;
    SIMU_TIME  index2time(Param const &, SIG_INDEX const) const;
};
