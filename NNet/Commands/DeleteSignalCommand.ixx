// DeleteSignalCommand.ixx
//
// Commands

module;

#include <memory>
#include <iostream>

export module NNetCommands:DeleteSignalCommand;

import :NNetCommand;

export class DeleteSignalCommand : public NNetCommand
{
public:

    DeleteSignalCommand(SignalId const& id)
        : m_signalId(id)
    {}

    void Do() final
    {
        m_upSignal = move(m_pNMWI->GetMonitorData().DeleteSignal(m_signalId));
        SignalSource const* pSigSrc { m_upSignal->GetSignalSource() };
        int iSigSrcType { m_upSignal->GetSigSrcType() };
        if (iSigSrcType != Signal::SIGSRC_GENERATOR)
        {
            Sensor const * pSensor { static_cast<Sensor const*>(pSigSrc) };
            m_sensorId = m_pNMWI->GetSensorList().GetSensorId(*pSensor);
            if (m_sensorId.IsNotNull())
                m_upSensor = move(m_pNMWI->GetSensorList().RemoveSensor(m_sensorId));
        }
        m_pSound->Play(L"DISAPPEAR_SOUND");
    };

    void Undo() final
    {
        m_pNMWI->GetMonitorData().AddSignal(m_signalId, move(m_upSignal));
        if (m_upSensor)
            m_pNMWI->GetSensorList().InsertSensor(move(m_upSensor), m_sensorId);
        m_pSound->Play(L"SNAP_IN_SOUND");
    };

    static void Register()
    {
        SymbolTable::ScrDefConst(NAME, &m_wrapper);
    }

    static void Push(SignalId const signalId)
    {
        if (IsTraceOn())
            TraceStream() << NAME << signalId << endl;
        PushCommand(make_unique<DeleteSignalCommand>(signalId));
    }

private:

    inline static const wstring NAME { L"DeleteSignal" };

    inline static struct Wrapper : public ScriptFunctor
    {
        void operator() (Script& script) const final
        {
            DeleteSignalCommand::Push(ScrReadSignalId(script));
        }
    } m_wrapper;

    SensorId           m_sensorId;
    SignalId           m_signalId;
    unique_ptr<Sensor> m_upSensor {};
    unique_ptr<Signal> m_upSignal {};
};
