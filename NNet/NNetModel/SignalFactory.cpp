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

SignalNr SignalFactory::addSignal
(
    NNetSignalSource         & sigSource,
    TrackNr            const   trackNr,
    NNetModelWriterInterface & nmwi
)
{
    unique_ptr<NNetSignal> upSignal { make_unique<NNetSignal>(* m_pObservable, sigSource) };   
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
    SignalNr     const signalNr { addSignal(*upSensor.get(), trackNr, nmwi) };
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
    SignalNr  const   signalNr { addSignal(*pSigGen, trackNr, nmwi) };
    return SignalId(trackNr, signalNr);
}

SignalId SignalFactory::MakeMicroSensorSignal
(
    NobId             const   nobId,
    TrackNr           const   trackNr,
    NNetModelWriterInterface& nmwi
)
{
    Nob          * pNob         { nmwi.GetNob(nobId) };
    MicroSensor  * pMicroSensor { pNob->CreateMicroSensor() };
    SignalNr const signalNr     { addSignal(*pMicroSensor, trackNr, nmwi) };
    return SignalId(trackNr, signalNr);
}
