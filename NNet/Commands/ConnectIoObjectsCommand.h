// ConnectIoObjectsCommand.h
//
// NNetCommands

#pragma once

#include "NobId.h"
#include "NNetModelWriterInterface.h"

using std::unique_ptr;

template <Nob_t PART, Nob_t RESULT>
class ConnectIoObjectsCommand
{
public:
    ConnectIoObjectsCommand
    (
        PART & nobAnimated, 
        PART & nobTarget
    )
      : m_nobTarget(nobTarget),
        m_nobAnimated(nobAnimated)
    {
        assert( m_nobAnimated.IsCompositeNob() == m_nobTarget.IsCompositeNob() );
        assert( m_nobAnimated.GetIoMode() != NobIoMode::internal );
        assert( m_nobTarget  .GetIoMode() != NobIoMode::internal );
        assert( m_nobTarget  .GetIoMode() !== m_nobAnimated.GetIoMode() );

        m_upResult = m_nobAnimated.IsInputNob()
            ? make_unique<RESULT>(m_nobTarget.GetPos(), m_nobAnimated, m_nobTarget  )
            : make_unique<RESULT>(m_nobTarget.GetPos(), m_nobTarget,   m_nobAnimated);
    }

    void Do( NNetModelWriterInterface& nmwi )
    {
        m_upResult->SetParentPointers();
        NobId id = nmwi.GetUPNobs().Push(move(m_upResult)); 
        m_upNobAnimated = nmwi.RemoveFromModel<PART>(m_nobAnimated);
        m_upNobTarget   = nmwi.RemoveFromModel<PART>(m_nobTarget);
        nmwi.GetNobPtr<RESULT *>(id)->Reconnect();
    }

    void Undo( NNetModelWriterInterface& nmwi )
    {
        m_upResult = nmwi.GetUPNobs().Pop<RESULT>();
        m_upResult->ClearParentPointers();
        m_upNobAnimated->Reconnect();
        m_upNobTarget  ->Reconnect();
        m_upNobAnimated = nmwi.ReplaceInModel<PART,PART>(move(m_upNobAnimated));
        m_upNobTarget   = nmwi.ReplaceInModel<PART,PART>(move(m_upNobTarget));
    }

private:
    PART             & m_nobTarget;
    PART             & m_nobAnimated;
    unique_ptr<PART>   m_upNobAnimated;
    unique_ptr<PART>   m_upNobTarget;
    unique_ptr<RESULT> m_upResult {};
};
