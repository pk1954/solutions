// PlugIoConnectors.ixx
//
// NNetCommands

export module NNetCommands:PlugIoConnectorsCmd;

import std;
import Debug;
import IoUtil;
import :NNetCommand;
import :NNetSound;

using std::vector;

export class PlugIoConnectorsCmd : public NNetCommand
{
public:
    PlugIoConnectorsCmd(NobId idAnimated, NobId idTarget)
    {
        IoConnector* pNobAnimated { m_pNMWI->GetNobPtr<IoConnector*>(idAnimated) };
        IoConnector* pNobTarget   { m_pNMWI->GetNobPtr<IoConnector*>(idTarget) };
        m_pInputConnector  = pNobAnimated->IsInputNob () ? pNobAnimated : pNobTarget;
        m_pOutputConnector = pNobAnimated->IsOutputNob() ? pNobAnimated : pNobTarget;
        Assert(m_pInputConnector->Size() == m_pOutputConnector->Size());

        m_size = m_pInputConnector->Size();
        for (size_t i = 0; i < m_size; ++i)
        {
            unique_ptr<Knot> upKnot{ make_unique<Knot>(m_pInputConnector->GetElem(i).GetPos()) };
            upKnot->AddIncoming(m_pOutputConnector->GetElem(i).GetPipe());
            upKnot->AddOutgoing(m_pInputConnector ->GetElem(i).GetPipe());
            m_upKnots.push_back(move(upKnot));
        }
        m_upOutputLines.resize(m_size);
        m_upInputLines .resize(m_size);
        m_pNMWI->CheckModel();
    }

    void Do() final
    {
        for (size_t i = 0; i < m_size; ++i)
        {
            m_pNMWI->Push2Model(move(m_upKnots.back()));
            m_upKnots.pop_back();
        }

        m_upOutputConnector = m_pNMWI->RemoveFromModel<IoConnector>(*m_pOutputConnector);
        m_upInputConnector  = m_pNMWI->RemoveFromModel<IoConnector>(*m_pInputConnector);
        for (size_t i = 0; i < m_size; ++i)
        {
            m_upOutputLines[i] = m_pNMWI->RemoveFromModel<IoLine>(m_pOutputConnector->GetElem(i));
            m_upInputLines [i] = m_pNMWI->RemoveFromModel<IoLine>(m_pInputConnector ->GetElem(i));
        }
    }

    void Undo() final
    {
        for (size_t i = 0; i < m_size; ++i)
            m_upKnots.push_back(m_pNMWI->PopFromModel<Knot>());

        m_upOutputConnector = m_pNMWI->ReplaceInModel<IoConnector>(move(m_upOutputConnector));
        m_upInputConnector  = m_pNMWI->ReplaceInModel<IoConnector>(move(m_upInputConnector));
        for (size_t i = 0; i < m_size; ++i)
        {
            m_upOutputLines[i] = m_pNMWI->ReplaceInModel<IoLine>(move(m_upOutputLines[i]));
            m_upInputLines [i] = m_pNMWI->ReplaceInModel<IoLine>(move(m_upInputLines [i]));
        }
    }

    static void Register()
    {
        SymbolTable::ScrDefConst(NAME, &m_wrapper);
    }

    static void Push(NobId const nobId1, NobId const nobId2)
    {
        if (IsTraceOn())
            TraceStream() << NAME << nobId1 << nobId2 << endl;
        PushCommand(make_unique<PlugIoConnectorsCmd>(nobId1, nobId2));
        PlaySound(SNAP_IN_SOUND);
    }

private:

    inline static const wstring NAME { L"PlugIoConnectors" };

    inline static struct myWrapper : public Wrapper
    {
        using Wrapper::Wrapper;
        void operator() (Script& script) const final
        {
            NobId const id1 { ScrReadNobId(script) };
            NobId const id2 { ScrReadNobId(script) };
            PlugIoConnectorsCmd::Push(id1, id2);
        }
    } m_wrapper { NAME };

    size_t m_size;

    IoConnector * m_pInputConnector;
    IoConnector * m_pOutputConnector;

    vector<unique_ptr<Knot>> m_upKnots;

    // take ownership of IoConnectors and IoLines between Do and Undo

    unique_ptr<IoConnector>    m_upInputConnector;
    unique_ptr<IoConnector>    m_upOutputConnector;
    vector<unique_ptr<IoLine>> m_upInputLines;
    vector<unique_ptr<IoLine>> m_upOutputLines;
};
