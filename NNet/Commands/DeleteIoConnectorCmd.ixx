// DeleteIoConnectorCmd.ixx
//
// Commands

module;

#include <memory>

export module DeleteIoConnectorCmd;

import CommandFunctions;
import Commands;
import NNetCommand;
import NNetCommandStack;
import NNetModel;

using std::unique_ptr;

export class DeleteIoConnectorCmd : public NNetCommand
{
public:
    explicit DeleteIoConnectorCmd(Nob& nob)
        : m_connector(*Cast2IoConnector(&nob))
    {}

    void Do() final
    {
        m_upIoConnector = m_pNMWI->RemoveFromModel<IoConnector>(m_connector);
        m_upIoConnector->DisconnectIoLines();
    }

    void Undo() final
    {
        m_upIoConnector->ConnectIoLines();
        m_pNMWI->Restore2Model(move(m_upIoConnector));
    }

private:

    IoConnector &           m_connector;
    unique_ptr<IoConnector> m_upIoConnector;
};
