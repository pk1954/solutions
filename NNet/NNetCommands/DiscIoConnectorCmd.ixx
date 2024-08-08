// DiscIoConnectorCmd.ixx
//
// NNetCommands

module;

#include <memory>
#include <iostream>

export module NNetCommands:DiscIoConnectorCmd;

import IoUtil;
import :NNetCommand;
import :NNetSound;
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
		PlaySound(UNLOCK_SOUND);
	}

    void Undo() final
    {
        m_upIoConnector->ConnectIoLines();
        m_pNMWI->Restore2Model(move(m_upIoConnector));
		PlaySound(SNAP_IN_SOUND);
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push(NobId nobId)
	{
		if (IsTraceOn())
			TraceStream() << NAME << nobId << endl;
		PushCommand(make_unique<DiscIoConnectorCmd>(nobId));
	}

private:

	inline static const wstring NAME { L"DiscIoConnector" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			DiscIoConnectorCmd::Push(ScrReadNobId(script));
		}
	} m_wrapper { NAME };

    IoConnector      const& m_connector;
    unique_ptr<IoConnector> m_upIoConnector {};
};
