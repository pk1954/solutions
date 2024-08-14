// PlugIoLines.ixx
//
// NNetCommands

export module NNetCommands:PlugIoLinesCmd;

import std;
import Debug;
import IoUtil;
import :NNetCommand;
import :NNetSound;

export class PlugIoLinesCmd : public NNetCommand
{
public:
    PlugIoLinesCmd
    (
        NobId idAnimated,
        NobId idTarget
    )
      : m_nobAnimated(*m_pNMWI->GetNobPtr<IoLine*>(idAnimated)),
        m_nobTarget  (*m_pNMWI->GetNobPtr<IoLine*>(idTarget))
    {
        Assert(m_nobAnimated.IsCompositeNob() == m_nobTarget.IsCompositeNob());
        Assert(m_nobAnimated.GetIoMode() != NobIoMode::internal);
        Assert(m_nobTarget  .GetIoMode() != NobIoMode::internal);
        Assert(m_nobTarget  .GetIoMode() != m_nobAnimated.GetIoMode());

        m_upKnot = make_unique<Knot>(m_nobTarget.GetPos());
        m_upKnot->AddIncoming((m_nobAnimated.IsOutputNob() ? m_nobAnimated : m_nobTarget).GetPipe());
        m_upKnot->AddOutgoing((m_nobAnimated.IsInputNob () ? m_nobAnimated : m_nobTarget).GetPipe());
    }

    void Do() final
    {
        m_pNMWI->Push2Model(move(m_upKnot)); 
        m_upNobAnimated = m_pNMWI->RemoveFromModel<IoLine>(m_nobAnimated);
        m_upNobTarget   = m_pNMWI->RemoveFromModel<IoLine>(m_nobTarget);
    }

    void Undo() final
    {
        m_upKnot = m_pNMWI->PopFromModel<Knot>();
        m_upNobAnimated->Reconnect();
        m_upNobTarget  ->Reconnect();
        m_upNobAnimated = m_pNMWI->ReplaceInModel<IoLine>(move(m_upNobAnimated));
        m_upNobTarget   = m_pNMWI->ReplaceInModel<IoLine>(move(m_upNobTarget));
        m_pNMWI->DeselectAllNobs();
    }

    static void Register()
    {
        SymbolTable::ScrDefConst(NAME, &m_wrapper);
    }

    static void Push(NobId const nobId1, NobId const nobId2)
    {
        if (IsTraceOn())
            TraceStream() << NAME << nobId1 << nobId2 << endl;
        PushCommand(make_unique<PlugIoLinesCmd>(nobId1, nobId2));
        PlaySound(SNAP_IN_SOUND);
    }

private:

    inline static const wstring NAME { L"PlugIoLines" };

    inline static struct myWrapper : public Wrapper
    {
        using Wrapper::Wrapper;
        void operator() (Script& script) const final
        {
            NobId const id1 { ScrReadNobId(script) };
            NobId const id2 { ScrReadNobId(script) };
            PlugIoLinesCmd::Push(id1, id2);
        }
    } m_wrapper { NAME };

    IoLine           & m_nobTarget;
    IoLine           & m_nobAnimated;
    unique_ptr<IoLine> m_upNobAnimated;
    unique_ptr<IoLine> m_upNobTarget;
    unique_ptr<Knot>   m_upKnot {};
};
