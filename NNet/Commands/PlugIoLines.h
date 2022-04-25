// PlugIoLines.h
//
// Commands

#pragma once

#include "NobId.h"
#include "Knot.h"
#include "NNetModelWriterInterface.h"
#include "NNetCommand.h"

using std::unique_ptr;

class PlugIoLines : public NNetCommand
{
public:
    PlugIoLines
    (
        IoLine       & connectorAnimated, 
        IoLine const & nobTarget
    )
      : m_nobTarget(nobTarget),
        m_nobAnimated(connectorAnimated)
    {
        assert(m_nobAnimated.IsCompositeNob() == m_nobTarget.IsCompositeNob());
        assert(m_nobAnimated.GetIoMode() != NobIoMode::internal);
        assert(m_nobTarget  .GetIoMode() != NobIoMode::internal);
        assert(m_nobTarget  .GetIoMode() != m_nobAnimated.GetIoMode());

        m_upKnot = make_unique<Knot>(m_nobTarget.GetPos());
        m_upKnot->SetIncoming(m_nobAnimated.IsOutputNob() ? m_nobAnimated : m_nobTarget);
        m_upKnot->SetOutgoing(m_nobAnimated.IsInputNob () ? m_nobAnimated : m_nobTarget);
    }

    void Do() final
    {
        m_pNMWI->Push2Model(move(m_upKnot)); 
        m_upNobAnimated = m_pNMWI->RemoveFromModel<IoLine>(m_nobAnimated);
        m_upNobTarget   = m_pNMWI->RemoveFromModel<IoLine>(m_nobTarget);
        (m_targetReachedFunc)();
    }

    void Undo() final
    {
        m_upKnot = m_pNMWI->PopFromModel<Knot>();
        m_upNobAnimated->Reconnect();
        m_upNobTarget  ->Reconnect();
        m_upNobAnimated = m_pNMWI->ReplaceInModel<IoLine>(move(m_upNobAnimated));
        m_upNobTarget   = m_pNMWI->ReplaceInModel<IoLine>(move(m_upNobTarget));
        m_pNMWI->DeselectAllNobs();
        (m_targetReachedFunc)();
    }

private:
    IoLine     const & m_nobTarget;
    IoLine           & m_nobAnimated;
    unique_ptr<IoLine> m_upNobAnimated;
    unique_ptr<IoLine> m_upNobTarget;
    unique_ptr<Knot>   m_upKnot {};
};
