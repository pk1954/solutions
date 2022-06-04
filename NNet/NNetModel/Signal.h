// Signal.h : 
//
// NNetModel

#pragma once

#include <vector>
#include <math.h>    
#include "observerInterface.h"
#include "MoreTypes.h"
#include "SimulationTime.h"
#include "NNetParameters.h"

using std::vector;
using std::unique_ptr;

using SIG_INDEX = long; 

class Param;
class Signal;
class BaseKnot;
class UPNobList;
class DrawContext;
class SignalSource;
class NNetModelReaderInterface;

class Signal : public ObserverInterface  // observes signal source 
{
public:

    Signal(Observable &, SignalSource &);

    ~Signal() override;

    void Reset(UPNobList const &);

    mV         GetDataPoint   (Param const &, fMicroSecs const) const;
    fMicroSecs FindNextMaximum(Param const &, fMicroSecs const) const;
    void       Draw           (DrawContext const &, bool const) const;
    void       WriteSignalInfo(wostream &)                      const;
    void       WriteSignalData(wostream &)                      const;

    bool Includes(MicroMeterPnt const) const;

    fMicroSecs GetStartTime() const { return m_timeStart; }
    void       SetStartTime(fMicroSecs const t)  { m_timeStart = t; }

    void Add(mV const);

    void Notify(bool const) final;

    void Check() const { /**/  };
    void Dump()  const;
    void CheckSignal() const;

    int GetSigSrcType() const { return m_iSourceType; }

    SignalSource const * GetSignalSource() const { return & m_sigSource; }

    inline static int const SIGSRC_CIRCLE    { 101 };
    inline static int const SIGSRC_GENERATOR { 102 };

private:

    inline static SIG_INDEX INVALID_SIG_INDEX { -1 };

    Observable   & m_dynModelObservable;
    SignalSource & m_sigSource;
    fMicroSecs     m_timeStart { SimulationTime::Get() };
    vector<mV>     m_data;
    int            m_iSourceType;

    SIG_INDEX  time2index(Param const &, fMicroSecs const) const;
    fMicroSecs index2time(Param const &, SIG_INDEX  const) const;
};
