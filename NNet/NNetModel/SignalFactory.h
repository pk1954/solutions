// SignalFactory.h : 
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "observable.h"
#include "SignalId.h"
#include "TrackNr.h"

class NNetModelWriterInterface;

class SignalFactory
{
public:

    static void Initialize(Observable & observable)
    {
        m_pObservable = & observable;
    }

    static SignalId MakeSensorSignal
    (
        MicroMeterCircle   const &, 
        TrackNr            const,
        NNetModelWriterInterface &
    );

private:

    inline static Observable * m_pObservable { nullptr };
};
