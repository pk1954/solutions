// MakeConnAnimation.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "win32_callable.h"
#include "win32_animation.h"
#include "IoConnector.h"
#include "IoNeuronList.h"
#include "NNetModelWriterInterface.h"
#include "MicroMeterPosDir.h"
#include "AnimationCmd.h"

using std::function;
using std::unique_ptr;
using std::make_unique;

class MainWindow;

class MakeConnAnimation : public AnimationCmd
{
public:
    MakeConnAnimation
    (
        MainWindow               & win,
        NNetModelWriterInterface & nmwi,
        unique_ptr<IoNeuronList>   upList
    )
      : AnimationCmd(win),
        m_nmwi(nmwi)
    {
        m_upIoConnector = make_unique<IoConnector>(move(upList));
    }

    virtual void Do(function<void()> const & targetReachedFunc)
    {
        m_nmwi.GetUPNobs().DeselectAllNobs();
        m_upIoConnector->SetParentPointers();
        m_nmwi.GetUPNobs().Push(move(m_upIoConnector));
        (targetReachedFunc)();
    }

    virtual void Undo(function<void()> const & targetReachedFunc)
    {
        m_upIoConnector = m_nmwi.GetUPNobs().Pop<IoConnector>();
        m_upIoConnector->ClearParentPointers();
        (targetReachedFunc)();
    }

private:

    unique_ptr<IoConnector>      m_upIoConnector {};  
    NNetModelWriterInterface & m_nmwi;
};