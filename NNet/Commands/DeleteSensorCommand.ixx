// DeleteSensorCommand.ixx
//
// Commands

module;

#include <memory>

export module DeleteSensorCommand;

import Types;
import NNetCommand;
import NNetModel;

using std::unique_ptr;

export class DeleteSensorCommand : public NNetCommand
{
public:

    DeleteSensorCommand(SensorId const& id)
        : m_sensorId(id)
    {}

    void Do() final
    {
        m_upSensor = move(m_pNMWI->GetSensorList().RemoveSensor(m_sensorId));
        m_signalId = m_pNMWI->FindSignalId
        (
            [this](Signal const &s) 
            { return s.GetSignalSource() == m_upSensor.get(); }
        );
        m_upSignal = move(m_pNMWI->GetMonitorData().DeleteSignal(m_signalId));
    };

    void Undo() final
    {
        m_pNMWI->GetSensorList().InsertSensor(move(m_upSensor), m_sensorId);
        m_pNMWI->GetMonitorData().AddSignal(m_signalId, move(m_upSignal));
    };

private:

    SensorId           m_sensorId {};
    SignalId           m_signalId {};
    unique_ptr<Sensor> m_upSensor {};
    unique_ptr<Signal> m_upSignal {};
};
