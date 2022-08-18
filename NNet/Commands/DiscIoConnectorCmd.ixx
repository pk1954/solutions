// DiscIoConnectorCmd.ixx
//
// Commands

module;

#include "NNetModelWriterInterface.h"
#include "NNetCommand.h"
#include "Nob.h"
#include "IoConnector.h"

export module DiscIoConnectorCmd;

using std::unique_ptr;

export class DiscIoConnectorCmd : public NNetCommand
{
public:
    explicit DiscIoConnectorCmd(Nob& nob)
        : m_connector(*Cast2IoConnector(&nob))
    {}

    void Do() final
    {
        m_upIoConnector = m_pNMWI->RemoveFromModel<IoConnector>(m_connector);
        m_upIoConnector->ClearParentPointers();
    }

    void Undo() final
    {
        m_upIoConnector->SetParentPointers();
        m_pNMWI->Restore2Model(move(m_upIoConnector));
    }

private:

    IoConnector     const& m_connector;
    unique_ptr<IoConnector> m_upIoConnector{};
};
