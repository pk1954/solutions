// MakeConnAnimation.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "win32_callable.h"
#include "win32_animation.h"
#include "InputConnector.h"
#include "OutputConnector.h"
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
        MainWindow                   & win,
        NNetModelWriterInterface     & nmwi,
        vector<IoNeuron *>          && list
   )
      : AnimationCmd(win),
        m_nmwi(nmwi)
    {
        if (list.front()->IsInputNeuron())
            m_upIoConnector = make_unique<InputConnector>(move(list));
        else 
            m_upIoConnector = make_unique<OutputConnector>(move(list));
    }

    virtual void Do(function<void()> const & targetReachedFunc)
    {
        m_nmwi.DeselectAllNobs();
        m_upIoConnector->SetParentPointers();
        m_nmwi.Push2Model(move(m_upIoConnector));
        (targetReachedFunc)();
    }

    virtual void Undo(function<void()> const & targetReachedFunc)
    {
        m_upIoConnector = m_nmwi.PopFromModel<IoConnector>();
        m_upIoConnector->ClearParentPointers();
        (targetReachedFunc)();
    }

private:

    unique_ptr<IoConnector>    m_upIoConnector {};  
    NNetModelWriterInterface & m_nmwi;
};