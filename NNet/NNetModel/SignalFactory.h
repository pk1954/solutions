// SignalFactory.h : 
//
// NNetModel

#pragma once

#include "SignalId.h"
#include "TrackNr.h"

import Observable;
import MoreTypes;

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

    static SignalId MakeSigGenSignal
    (
        TrackNr            const,
        NNetModelWriterInterface &
    );

private:

    static SignalNr addSignal
    (
        SignalSource &,
        TrackNr const,
        NNetModelWriterInterface &
    );

    inline static Observable * m_pObservable { nullptr };
};
