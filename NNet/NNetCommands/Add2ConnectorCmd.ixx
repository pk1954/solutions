// Add2ConnectorCmd.ixx
//
// NNetCommands

module;

#include <vector>
#include <iostream>

export module NNetCommands:Add2ConnectorCmd;

import :SelectionCommand;

using std::vector;

export class Add2ConnectorCmd : public SelectionCommand
{
public:
    Add2ConnectorCmd(NobId const, NobId const);

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
		PushCommand(make_unique<Add2ConnectorCmd>(nobId1, nobId2));
        PlaySound(L"SNAP_IN_SOUND");
    }

private:

    inline static const wstring NAME { L"Add2Connector" };

    inline static struct myWrapper : public Wrapper
    {
        using Wrapper::Wrapper;
        void operator() (Script& script) const final
        {
            NobId const id1 { ScrReadNobId(script) };
            NobId const id2 { ScrReadNobId(script) };
            Add2ConnectorCmd::Push(id1, id2);
        }
    } m_wrapper { NAME };

    NobId m_id1;
    NobId m_id2;

    MicroMeterLine        m_umLine;
    vector<MicroMeterPnt> m_originalPositions;

    unique_ptr<IoConnector> m_upIoConnector1;
    unique_ptr<IoConnector> m_upIoConnector2;
    unique_ptr<IoConnector> m_upIoConnectorResult;

    void add2IoLines   (vector<IoLine*> &,        NobId const);
    void sortIoLines   (vector<IoLine*> &,        MicroMeterLine const&);
    void alignPositions(vector<IoLine*> const&,   MicroMeterLine const&);
    void remove (unique_ptr<IoConnector>&, NobId const);
    void restore(unique_ptr<IoConnector>&, NobId const);
};