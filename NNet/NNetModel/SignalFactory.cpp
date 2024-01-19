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
    SignalNr         const signalNr { nmwi.GetMonitorData().AddSignal(trackNr, move(upSignal)) };
    return signalNr;
}

SignalId SignalFactory::MakeSensorSignal
(
    MicroMeterCircle   const & umCircle,
    TrackNr            const   trackNr,
    NNetModelWriterInterface & nmwi
)
{
    unique_ptr<Sensor> upSensor { make_unique<Sensor>(umCircle, nmwi.GetUPNobsC()) };        
    SignalNr     const signalNr { addSignal2Monitor(upSensor.get(), trackNr, nmwi) };
    nmwi.GetSensorList().PushSensor(move(upSensor));                                         
    return SignalId(trackNr, signalNr);
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
