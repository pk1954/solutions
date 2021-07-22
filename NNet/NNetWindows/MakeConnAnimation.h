// MakeConnAnimation.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "win32_callable.h"
#include "win32_animation.h"
#include "Connector.h"
#include "IoNeuronList.h"
#include "NNetModelWriterInterface.h"
#include "MicroMeterPosDir.h"
#include "AnimationCmd.h"

using std::function;
using std::make_unique;

class MainWindow;

class MakeConnAnimation : public AnimationCmd
{
public:
    MakeConnAnimation
    (
        MainWindow               & win,
        NNetModelWriterInterface & nmwi,
        IoNeuronList             & list
    )
      : AnimationCmd(win),
        m_nmwi(nmwi)
    {
        m_upConnector = make_unique<Connector>(list);
    }

    virtual void Do(function<void()> const & targetReachedFunc)
    {
        m_nmwi.GetUPNobs().DeselectAllNobs();
        m_upConnector->SetParentPointers();
        m_nmwi.GetUPNobs().Push(move(m_upConnector));
        (targetReachedFunc)();
    }

    virtual void Undo(function<void()> const & targetReachedFunc)
    {
        m_upConnector = m_nmwi.GetUPNobs().Pop<Connector>();
        m_upConnector->ClearParentPointers();
        (targetReachedFunc)();
    }

private:

    unique_ptr<Connector>      m_upConnector  {};  
    NNetModelWriterInterface & m_nmwi;
};