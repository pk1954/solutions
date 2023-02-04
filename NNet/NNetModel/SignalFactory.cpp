// SignalFactory.cpp 
//
// NNetModel

module;

#include <memory>

module NNetModel:SignalFactory;

import Types;
import :TrackNr;
import :SignalNr;
import :NNetModelWriterInterface;
import :MicroSensor;
import :MacroSensor;
import :SignalGenerator;
import :SignalSource;
import :Signal;

using std::unique_ptr;
using std::make_unique;

SignalNr SignalFactory::addSignal
(
    SignalSource             & sigSource,
    TrackNr            const   trackNr,
    NNetModelWriterInterface & nmwi
)
{
    unique_ptr<Signal> upSignal { make_unique<Signal>(* m_pObservable, sigSource) };   
    SignalNr     const signalNr { nmwi.GetMonitorData().AddSignal(trackNr, move(upSignal)) };
    return signalNr;
}

SignalId SignalFactory::MakeSensorSignal
(
    MicroMeterCircle   const & umCircle,
    TrackNr            const   trackNr,
    NNetModelWriterInterface & nmwi
)
{
    unique_ptr<MacroSensor> upSensor { make_unique<MacroSensor>(umCircle, nmwi.GetUPNobsC()) };        
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
    Nob *                   pNob          { nmwi.GetNob(nobId) };
    unique_ptr<MicroSensor> upMicroSensor { make_unique<MicroSensor>(pNob) };
    SignalNr          const signalNr      { addSignal(*upMicroSensor.get(), trackNr, nmwi) };
    nmwi.GetSensorList().PushSensor(move(upMicroSensor));
    return SignalId(trackNr, signalNr);
}
