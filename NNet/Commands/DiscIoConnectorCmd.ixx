// DiscIoConnectorCmd.ixx
//
// Commands

module;

#include <memory>
#include <iostream>

export module DiscIoConnectorCmd;

import NNetCommand;
import NNetModel;

using std::unique_ptr;
using std::unique_ptr;

export class DiscIoConnectorCmd : public NNetCommand
{
public:
    explicit DiscIoConnectorCmd(NobId id)
        : m_connector(*m_pNMWI->GetNobPtr<IoConnector*>(id))
    {}

    void Do() final
    {
        m_upIoConnector = m_pNMWI->RemoveFromModel<IoConnector>(m_connector);
        m_upIoConnector->DisconnectIoLines();
		m_pSound->Play(L"UNLOCK_SOUND");
	}

    void Undo() final
    {
        m_upIoConnector->ConnectIoLines();
        m_pNMWI->Restore2Model(move(m_upIoConnector));
		m_pSound->Play(L"SNAP_IN_SOUND");
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, new Wrapper);
	}

	static void Push(NobId nobId)
	{
		if (IsTraceOn())
			TraceStream() << NAME << nobId.GetValue() << endl;
		m_pStack->PushCommand(make_unique<DiscIoConnectorCmd>(nobId));
	}

private:

	inline static const wstring NAME { L"DiscIoConnector" };

	class Wrapper : public ScriptFunctor
	{
	public:
		void operator() (Script& script) const final
		{
			DiscIoConnectorCmd::Push(ScrReadNobId(script));
		}
	};

    IoConnector      const& m_connector;
    unique_ptr<IoConnector> m_upIoConnector {};
};
