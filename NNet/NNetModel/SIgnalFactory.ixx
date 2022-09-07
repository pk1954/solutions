// SignalFactory.ixx
//
// NNetModel

module;

#include "NNetModelWriterInterface.h"
#include "SignalSource.h"

export module SignalFactory;

import Observable;
import Types;
import TrackNr;
import SignalId;
import SignalNr;

export class SignalFactory
{
public:

    static void Initialize(Observable& observable)
    {
        m_pObservable = &observable;
    }

    static SignalId MakeSensorSignal
    (
        MicroMeterCircle   const&,
        TrackNr            const,
        NNetModelWriterInterface&
    );

    static SignalId MakeSigGenSignal
    (
        TrackNr            const,
        NNetModelWriterInterface&
    );

private:

    static SignalNr addSignal
    (
        SignalSource&,
        TrackNr const,
        NNetModelWriterInterface&
    );

    inline static Observable* m_pObservable{ nullptr };
};
