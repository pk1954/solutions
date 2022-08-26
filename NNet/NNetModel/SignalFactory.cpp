// SignalFactory.cpp 
//
// NNetModel

#include <memory>
#include "NNetModelWriterInterface.h"
#include "MonitorData.h"
#include "SignalNr.h"
#include "SignalSource.h"
#include "SignalGenerator.h"
#include "SignalFactory.h"

import TrackNr;
import SignalId;

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
    SignalGenerator * pSigGen { nmwi.GetSigGenSelected() };
    SignalNr   const signalNr { addSignal(*pSigGen, trackNr, nmwi) };
    return SignalId(trackNr, signalNr);
}
