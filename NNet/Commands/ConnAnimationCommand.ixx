// ConnAnimationCommand.ixx
//
// Commands

module;

#include <vector>
#include <iostream>

export module NNetCommands:ConnAnimationCommand;

import :NNetCommand;

using std::vector;

export class ConnAnimationCommand : public NNetCommand
{
public:
    ConnAnimationCommand(NobId const, NobId const);

    void Do() final
    {
        NNetCommand::Do();
        if (m_pNMWI->IsIoConnector(m_id1))
            m_upIoConnector1 = m_pNMWI->RemoveFromModel<IoConnector>(m_id1);
        if (m_pNMWI->IsIoConnector(m_id2))
            m_upIoConnector2 = m_pNMWI->RemoveFromModel<IoConnector>(m_id2);
    }

    void Undo() final
    {
        if (m_upIoConnector1)
            m_pNMWI->Restore2Model(move(m_upIoConnector1));
        if (m_upIoConnector2)
            m_pNMWI->Restore2Model(move(m_upIoConnector2));
        NNetCommand::Undo();
    }

    bool IsAsyncCommand() final { return true; };

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push(NobId const nobId1, NobId const nobId2)
	{
		if (IsTraceOn())
			TraceStream() << NAME << nobId1.GetValue() << L' ' << nobId2.GetValue() << endl;
		m_pStack->PushCommand(make_unique<ConnAnimationCommand>(nobId1, nobId2));
	}

private:

    inline static const wstring NAME { L"ConnAnimation" };

    inline static struct Wrapper : public ScriptFunctor
    {
        void operator() (Script& script) const final
        {
            NobId const id1 { ScrReadNobId(script) };
            NobId const id2 { ScrReadNobId(script) };
            ConnAnimationCommand::Push(id1, id2);
        }
    } m_wrapper;

    NobId m_id1;
    NobId m_id2;

    vector<IoLine*> m_nobsAnimated{};

    unique_ptr<IoConnector> m_upIoConnector1;
    unique_ptr<IoConnector> m_upIoConnector2;

    void add2nobsAnimated(NobId const);
    void sortNobsAnimated(MicroMeterLine const&);
};