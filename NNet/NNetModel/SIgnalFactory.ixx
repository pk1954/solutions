// SignalFactory.ixx
//
// NNetModel

module;

#include <compare>

export module NNetModel:SignalFactory;

import Observable;
import Types;
import :TrackNr;
import :SignalNr;
import :SignalId;
import :SignalSource;
import :NNetModelWriterInterface;

export class SignalFactory
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
        TrackNr const,
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
