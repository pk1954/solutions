// DelSensorCmd.ixx
//
// NNetCommands

module;

#include <iostream>

export module NNetCommands:DelSensorCmd;

import Commands;
import Types;
import IoUtil;
import NNetModel;
import Signals;
import :NNetCommand;
import :NNetSound;

export class DelSensorCmd : public NNetCommand
{
public:

    DelSensorCmd(SensorId const& id)
        : m_sensorId(id)
    {
        NNetSignalSource const * const pSigSrc { m_pNMWI->GetSensorList().GetSensor(id) };
        m_signalId = m_pNMWI->FindSignalId(pSigSrc);
    }

    void Do() final
    {
        m_upSensor = move(m_pNMWI->RemoveSensor(m_sensorId));
        m_upSignal = move(m_pNMWI->RemoveSignal(m_signalId));
        PlaySound(DISAPPEAR_SOUND);
    };

    void Undo() final
    {
        m_pNMWI->AddSensor(move(m_upSensor), m_sensorId);
        m_pNMWI->AddSignal(move(m_upSignal), m_signalId);
        PlaySound(SNAP_IN_SOUND);
    };

    static void Register()
    {
        SymbolTable::ScrDefConst(NAME, &m_wrapper);
    }

    static void Push(SensorId id)
    {
        if (IsTraceOn())
            TraceStream() << NAME << id << endl;
        PushCommand(make_unique<DelSensorCmd>(id));
    }

private:

    inline static const wstring NAME { L"DeleteSensor" };

    inline static struct myWrapper : public Wrapper
    {
        using Wrapper::Wrapper;
        void operator() (Script& script) const final
        {
            SensorId const id { script.ScrReadInt() };
            DelSensorCmd::Push(id);
        }
    } m_wrapper { NAME };

    SensorId           m_sensorId {};
    SignalId           m_signalId {};
    unique_ptr<Sensor> m_upSensor {};
    unique_ptr<Signal> m_upSignal {};
};
