// Add2Connector.ixx
//
// NNetCommands

module;

#include <vector>
#include <iostream>

export module NNetCommands:Add2Connector;

import :NNetCommand;

using std::vector;

export class Add2Connector : public NNetCommand
{
public:
    Add2Connector(NobId const, NobId const);

    void Do  () final;
    void Undo() final;

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push(NobId const nobId1, NobId const nobId2)
	{
		if (IsTraceOn())
			TraceStream() << NAME << nobId1 << nobId2 << endl;
		PushCommand(make_unique<Add2Connector>(nobId1, nobId2));
        PlaySound(L"SNAP_IN_SOUND");
    }

private:

    inline static const wstring NAME { L"ConnAnimation" };

    inline static struct myWrapper : public Wrapper
    {
        using Wrapper::Wrapper;
        void operator() (Script& script) const final
        {
            NobId const id1 { ScrReadNobId(script) };
            NobId const id2 { ScrReadNobId(script) };
            Add2Connector::Push(id1, id2);
        }
    } m_wrapper { NAME };

    NobId m_id1;
    NobId m_id2;

    unique_ptr<IoConnector> m_upIoConnector1;
    unique_ptr<IoConnector> m_upIoConnector2;
    unique_ptr<IoConnector> m_upIoConnectorResult;

    void add2IoLines(NobId const,     vector<IoLine*> &);
    void align(MicroMeterLine const&, vector<IoLine*> const&);
};