// DeleteSensorCmd.ixx
//
// Commands

module;

#include <iostream>

export module DeleteSensorCmd;

import Types;
import NNetCommand;
import NNetModel;

export class DeleteSensorCmd : public NNetCommand
{
public:

    DeleteSensorCmd(SensorId const& id)
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
        m_pSound->Play(L"DISAPPEAR_SOUND");
    };

    void Undo() final
    {
        m_pNMWI->GetSensorList().InsertSensor(move(m_upSensor), m_sensorId);
        m_pNMWI->GetMonitorData().AddSignal(m_signalId, move(m_upSignal));
        m_pSound->Play(L"SNAP_IN_SOUND");
    };

    static void Register()
    {
        SymbolTable::ScrDefConst(NAME, new Wrapper);
    }

    static void Push(SensorId id)
    {
        if (IsTraceOn())
            TraceStream() << NAME << id << endl;
        m_pStack->PushCommand(make_unique<DeleteSensorCmd>(id));
    }

private:

    inline static const wstring NAME { L"DeleteSensor" };

    class Wrapper : public ScriptFunctor
    {
    public:
        void operator() (Script& script) const final
        {
            SensorId const id { script.ScrReadInt() };
            DeleteSensorCmd::Push(id);
        }
    };

    SensorId           m_sensorId {};
    SignalId           m_signalId {};
    unique_ptr<Sensor> m_upSensor {};
    unique_ptr<Signal> m_upSignal {};
};
