// PlugIoNeurons.h
//
// NNetWindows

#pragma once

#include "NobId.h"
#include "NNetModelWriterInterface.h"
#include "AnimationCmd.h"

using std::unique_ptr;

class PlugIoNeurons : public AnimationCmd
{
public:
    PlugIoNeurons
    (
        IoNeuron   & connectorAnimated, 
        IoNeuron   & nobTarget,
        MainWindow & win
   )
      : AnimationCmd(win),
        m_nobTarget(nobTarget),
        m_nobAnimated(connectorAnimated),
        m_win(win)
    {
        assert(m_nobAnimated.IsCompositeNob() == m_nobTarget.IsCompositeNob());
        assert(m_nobAnimated.GetIoMode() != NobIoMode::internal);
        assert(m_nobTarget  .GetIoMode() != NobIoMode::internal);
        assert(m_nobTarget  .GetIoMode() !== m_nobAnimated.GetIoMode());

        m_upNeuron = make_unique<Neuron>(m_nobTarget.GetPos());
        m_upNeuron->SetIncoming(m_nobAnimated.IsOutputNob() ? m_nobAnimated : m_nobTarget);
        m_upNeuron->SetOutgoing(m_nobAnimated.IsInputNob () ? m_nobAnimated : m_nobTarget);
    }

    virtual void DoAnimation(function<void()> const & targetReachedFunc)
    {
        m_pNMWI->Push2Model(move(m_upNeuron)); 
        m_upNobAnimated = m_pNMWI->RemoveFromModel<IoNeuron>(m_nobAnimated);
        m_upNobTarget   = m_pNMWI->RemoveFromModel<IoNeuron>(m_nobTarget);
        if (targetReachedFunc)
            (targetReachedFunc)();
    }

    virtual void UndoAnimation(function<void()> const & targetReachedFunc)
    {
        m_upNeuron = m_pNMWI->PopFromModel<Neuron>();
        m_upNobAnimated->Reconnect();
        m_upNobTarget  ->Reconnect();
        m_upNobAnimated = m_pNMWI->ReplaceInModel<IoNeuron,IoNeuron>(move(m_upNobAnimated));
        m_upNobTarget   = m_pNMWI->ReplaceInModel<IoNeuron,IoNeuron>(move(m_upNobTarget));
        m_pNMWI->DeselectAllNobs();
        if (targetReachedFunc)
            (targetReachedFunc)();
    }

private:
    IoNeuron     const & m_nobTarget;
    IoNeuron           & m_nobAnimated;
    unique_ptr<IoNeuron> m_upNobAnimated;
    unique_ptr<IoNeuron> m_upNobTarget;
    unique_ptr<Neuron>   m_upNeuron {};
    MainWindow         & m_win;
};
