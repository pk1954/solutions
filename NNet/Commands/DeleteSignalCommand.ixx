// DeleteSignalCommand.ixx
//
// Commands

module;

#include <memory>

export module DeleteSignalCommand;

import Types;
import NNetCommand;
import NNetModel;

using std::unique_ptr;

export class DeleteSignalCommand : public NNetCommand
{
public:

    DeleteSignalCommand(SignalId const& id)
        : m_signalId(id)
    {}

    void Do() final
    {
        m_upSignal = move(m_pNMWI->GetMonitorData().DeleteSignal(m_signalId));
        if (m_upSignal->GetSigSrcType() == Signal::SIGSRC_CIRCLE)
        {
            SignalSource const* pSigSrc = m_upSignal->GetSignalSource();
            Sensor       const* pSensor { static_cast<Sensor const *>(pSigSrc) };
            m_sensorId = m_pNMWI->GetSensorList().GetSensorId(*pSensor);
            m_upSensor = move(m_pNMWI->GetSensorList().RemoveSensor(m_sensorId));
        }
    };

    void Undo() final
    {
        m_pNMWI->GetMonitorData().AddSignal(m_signalId, move(m_upSignal));
        if (m_upSensor)
            m_pNMWI->GetSensorList().InsertSensor(move(m_upSensor), m_sensorId);
    };

private:

    SensorId           m_sensorId;
    SignalId           m_signalId;
    unique_ptr<Sensor> m_upSensor {};
    unique_ptr<Signal> m_upSignal {};
};
