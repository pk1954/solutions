// DeleteSignalCommand.ixx
//
// Commands

module;

#include "NNetModelWriterInterface.h"
#include "MonitorData.h"

export module DeleteSignalCommand;

import MoreTypes;
import NNetCommand;

export class DeleteSignalCommand : public NNetCommand
{
public:

    DeleteSignalCommand(SignalId const& id)
        : m_signalId(id)
    {}

    void Do() final
    {
        m_upSignal = move(m_pNMWI->GetMonitorData().DeleteSignal(m_signalId));
    };

    void Undo() final
    {
        m_pNMWI->GetMonitorData().AddSignal(m_signalId, move(m_upSignal));
    };

private:

    SignalId           m_signalId{};
    unique_ptr<Signal> m_upSignal{};
};
