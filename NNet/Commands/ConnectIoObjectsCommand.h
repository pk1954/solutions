// ConnectIoObjectsCommand.h
//
// NNetCommands

#pragma once

#include "NobId.h"
#include "NNetModelWriterInterface.h"
#include "AnimationCmd.h"

using std::unique_ptr;

template <Nob_t PART, Nob_t RESULT>
class ConnectIoObjectsCommand : public AnimationCmd
{
public:
    ConnectIoObjectsCommand
    (
        NNetModelWriterInterface & nmwi,
        PART                     & nobAnimated, 
        PART                     & nobTarget,
        MainWindow               & win
    )
      : AnimationCmd(win),
        m_nmwi(nmwi),
        m_nobTarget(nobTarget),
        m_nobAnimated(nobAnimated),
        m_win(win)
    {
        assert( m_nobAnimated.IsCompositeNob() == m_nobTarget.IsCompositeNob() );
        assert( m_nobAnimated.GetIoMode() != NobIoMode::internal );
        assert( m_nobTarget  .GetIoMode() != NobIoMode::internal );
        assert( m_nobTarget  .GetIoMode() !== m_nobAnimated.GetIoMode() );

        m_upResult = m_nobAnimated.IsInputNob()
        ? make_unique<RESULT>(m_nobTarget.GetPos(), m_nobAnimated, m_nobTarget  )
        : make_unique<RESULT>(m_nobTarget.GetPos(), m_nobTarget,   m_nobAnimated);
    }

    virtual void Do(function<void()> const & targetReachedFunc)
    {
        m_upResult->SetParentPointers();
        m_nmwi.Push2Model(move(m_upResult)); 
        m_upNobAnimated = m_nmwi.RemoveFromModel<PART>(m_nobAnimated);
        m_upNobTarget   = m_nmwi.RemoveFromModel<PART>(m_nobTarget);
        if (targetReachedFunc)
            (targetReachedFunc)();
    }

    virtual void Undo(function<void()> const & targetReachedFunc)
    {
        m_upResult = m_nmwi.PopFromModel<RESULT>();
        m_upResult->ClearParentPointers();
        m_upNobAnimated->Reconnect();
        m_upNobTarget  ->Reconnect();
        m_upNobAnimated = m_nmwi.ReplaceInModel<PART,PART>(move(m_upNobAnimated));
        m_upNobTarget   = m_nmwi.ReplaceInModel<PART,PART>(move(m_upNobTarget));
        m_nmwi.GetUPNobs().DeselectAllNobs();
        if (targetReachedFunc)
            (targetReachedFunc)();
    }

private:
    NNetModelWriterInterface & m_nmwi;
    PART                     & m_nobTarget;
    PART                     & m_nobAnimated;
    unique_ptr<PART>           m_upNobAnimated;
    unique_ptr<PART>           m_upNobTarget;
    unique_ptr<RESULT>         m_upResult {};
    MainWindow               & m_win;
};
