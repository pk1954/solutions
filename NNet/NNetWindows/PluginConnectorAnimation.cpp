// PluginConnectorAnimation.cpp
//
// NNetWindows

#include "stdafx.h"
#include "Resource.h"
#include "MoreTypes.h"
#include "CalcOrthoVector.h"
#include "win32_Commands.h"
#include "CommandStack.h"
#include "NobIdList.h"
#include "NNetModelCommands.h"
#include "NNetModelWriterInterface.h"
#include "PluginConnectorAnimation.h"

using std::make_unique;

PluginConnectorAnimation::PluginConnectorAnimation
( 
    Connector   & nobAnimated,
    Connector   & nobTarget,
    MainWindow  & win,
    WinCommands & cmds
)
  : AnimatedCommand(win, cmds),
    m_nobTarget(nobTarget),
    m_nobAnimated(nobAnimated)
{
    assert( m_nobAnimated.IsCompositeNob() == m_nobTarget.IsCompositeNob() );
    assert( m_nobAnimated.GetIoMode() != NobIoMode::internal );
    assert( m_nobTarget  .GetIoMode() != NobIoMode::internal );
    assert( m_nobTarget  .GetIoMode() !== m_nobAnimated.GetIoMode() );

    Radian        const radianTarget { m_nobTarget.GetDir() };
    MicroMeterPnt const umDirVector  { Radian2Vector(radianTarget).ScaledTo(NEURON_RADIUS) };
    
    m_umPosDirTarget.push_back(m_nobAnimated.GetPosDir());

    array <float, 2> fOffsets { 5.0f, 1.4f };

    for (size_t i = 1; i<= 2; ++i )
    {
        float fOff { fOffsets[i-1] };
        if (m_nobTarget.GetIoMode() == NobIoMode::input )
            fOff = -fOff;
        MicroMeterPnt const umPosOffset { umDirVector * fOff };
        MicroMeterPnt const umPosTarget { m_nobTarget.GetPos() + umPosOffset };
        m_umPosDirTarget.push_back(MicroMeterPosDir(umPosTarget, radianTarget));
    }

    m_upClosedNob = m_nobAnimated.IsInputNob()
                    ? make_unique<ClosedConnector>(m_nobAnimated, m_nobTarget)
                    : make_unique<ClosedConnector>(m_nobTarget,   m_nobAnimated);
}

void PluginConnectorAnimation::updateUI()  // runs in animation thread
{
    m_nobAnimated.SetPosDir(m_animation.GetActual());
    m_win.Notify(false);
}

void PluginConnectorAnimation::nextAnimationPhase(Mode const mode) // runs in UI thread
{
    MicroMeterPosDir umPosDirStart { m_nobAnimated.GetPosDir() };
    MicroMeterPosDir umPosDirTarget;

    if (mode == Mode::mode_do)
    {
        switch (m_iPhase++)
        {
        case 1:  BlockUI();
                 umPosDirTarget = m_umPosDirTarget[1]; break;
        case 2:	 umPosDirTarget = m_umPosDirTarget[2]; break;
        case 3:  m_upClosedNob->SetParentPointers();
                 m_NMWI.GetUPNobs().Push(move(m_upClosedNob));
                 UnblockUI();
                 return; 
        default: return;        // do not start animation
        }
    }
    else
    {
        switch (m_iPhase--)
        {
        case 3:  BlockUI();
                 m_upClosedNob = m_NMWI.GetUPNobs().Pop<ClosedConnector>();
                 m_upClosedNob->ClearParentPointers();
                 [[fallthrough]]; 
        case 2:  umPosDirTarget = m_umPosDirTarget[1]; break;
        case 1:	 umPosDirTarget = m_umPosDirTarget[0]; break;
        case 0:  UnblockUI();
                 return; 
        default: return;                // do not start animation
        }
    }
    StartAnimation(umPosDirStart, umPosDirTarget);
}
