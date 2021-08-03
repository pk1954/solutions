// DeleteSignalCommand.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "MonitorData.h"
#include "Command.h"

class DeleteSignalCommand: public Command
{
public:

    DeleteSignalCommand(SignalId const id)
        : m_signalId(id)
    {}

    virtual void Do(NNetModelWriterInterface & nmwi) 
    {
        MonitorData & monitorData { nmwi.GetMonitorData() };
        m_upSignal = move(monitorData.DeleteSignal(m_signalId));
        m_signalIdOldSelected = monitorData.ResetSelectedSignal();
    };

    virtual void Undo(NNetModelWriterInterface & nmwi)
    { 
        MonitorData & monitorData { nmwi.GetMonitorData() };
        monitorData.AddSignal(m_signalId.GetTrackNr(), move(m_upSignal));
        monitorData.SetSelectedSignalId(m_signalIdOldSelected);
    };

private:

    SignalId           m_signalId;
    SignalId           m_signalIdOldSelected;
    unique_ptr<Signal> m_upSignal {};
};
