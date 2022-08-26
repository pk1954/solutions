// SignalFactory.h : 
//
// NNetModel

#pragma once

#include "NNetModelWriterInterface.h"
#include "SignalSource.h"

import Observable;
import MoreTypes;
import TrackNr;
import SignalId;
import SignalNr;

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
