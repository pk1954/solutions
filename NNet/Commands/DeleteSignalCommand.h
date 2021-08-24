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

    DeleteSignalCommand(SignalId const & id)
      :  m_signalId(id)
    {}

    virtual void Do(NNetModelWriterInterface & nmwi) 
    {
        m_upSignal = move(nmwi.GetMonitorData().DeleteSignal(m_signalId));
    };

    virtual void Undo(NNetModelWriterInterface & nmwi)
    { 
        nmwi.GetMonitorData().AddSignal(m_signalId, move(m_upSignal));
    };

private:

    SignalId           m_signalId {};
    unique_ptr<Signal> m_upSignal {};
};
