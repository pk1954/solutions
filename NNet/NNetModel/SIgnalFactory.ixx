// SignalFactory.ixx
//
// NNetModel

export module NNetModel:SignalFactory;

import Observable;
import Types;
import Signals;
import :NNetSignalSource;
import :NNetModelWriterInterface;

export class SignalFactory
{
public:

    static SignalId MakeSigGenSignal
    (
        TrackNr const,
        NNetModelWriterInterface&
    );

private:

    static SignalNr addSignal2Monitor( NNetSignalSource * const,TrackNr const, NNetModelWriterInterface&);
};
