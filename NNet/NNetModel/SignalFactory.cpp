// SignalFactory.cpp 
//
// NNetModel

module;

#include <memory>

module NNetModel:SignalFactory;

import Types;
import Signals;
import :NNetModelWriterInterface;
import :NNetSignal;
import :MicroSensor;
import :Sensor;
import :SignalGenerator;
import :NNetSignalSource;

using std::unique_ptr;
using std::make_unique;

SignalNr SignalFactory::addSignal2Monitor
(
    NNetSignalSource * const   pSigSource,
    TrackNr            const   trackNr,
    NNetModelWriterInterface & nmwi
)
{
    unique_ptr<NNetSignal> upSignal { make_unique<NNetSignal>(pSigSource) };   
    SignalNr         const signalNr { nmwi.GetMonitorData().AddSignal(move(upSignal), trackNr) };
    return signalNr;
}

SignalId SignalFactory::MakeSigGenSignal
(
    TrackNr            const   trackNr,
    NNetModelWriterInterface & nmwi
)
{
    SignalGenerator * pSigGen  { nmwi.GetSigGenSelected() };
    SignalNr  const   signalNr { addSignal2Monitor(pSigGen, trackNr, nmwi) };
    return SignalId(trackNr, signalNr);
}
