// SignalFactory.cpp 
//
// NNetModel

#include "stdafx.h"
#include "NNetModelWriterInterface.h"
#include "MonitorData.h"
#include "UPSensorList.h"
#include "SignalFactory.h"

using std::unique_ptr;
using std::make_unique;

SignalId SignalFactory::MakeSensorSignal
(
    MicroMeterCircle   const & umCircle,
    TrackNr            const   trackNr,
    NNetModelWriterInterface & nmwi
)
{
    unique_ptr<Sensor> upSensor { make_unique<Sensor>(umCircle, nmwi.GetUPNobsC()) };        
    unique_ptr<Signal> upSignal { make_unique<Signal>(* m_pObservable, *upSensor.get()) };   
    SignalNr     const signalNr { nmwi.GetMonitorData().AddSignal(trackNr, move(upSignal)) };
    nmwi.GetSensorList().PushSensor(move(upSensor));                                         
    return SignalId(trackNr, signalNr);
}
