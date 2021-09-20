// MakeConnAnimation.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "win32_animation.h"
#include "InputConnector.h"
#include "OutputConnector.h"
#include "NNetModelWriterInterface.h"
#include "MicroMeterPosDir.h"
#include "NNetCommand.h"

using std::function;
using std::unique_ptr;
using std::make_unique;

class MainWindow;

class MakeConnAnimation : public NNetCommand
{
public:
    MakeConnAnimation(vector<IoNeuron *> && list)
    {
        if (list.front()->IsInputNeuron())
            m_upIoConnector = make_unique<InputConnector>(move(list));
        else 
            m_upIoConnector = make_unique<OutputConnector>(move(list));
    }

    virtual void DoAnimation()
    {
//        wcout << L'#' << __FUNCDNAME__ << endl;
        m_pNMWI->DeselectAllNobs();
        m_upIoConnector->SetParentPointers();
        m_pNMWI->Push2Model(move(m_upIoConnector));
        (m_targetReachedFunc)();
    }

    virtual void UndoAnimation()
    {
//        wcout << L'#' << __FUNCDNAME__ << endl;
        m_upIoConnector = move(m_pNMWI->PopFromModel<IoConnector>());
        m_upIoConnector->ClearParentPointers();
        m_upIoConnector->UnlockDirection();
        (m_targetReachedFunc)();
    }

private:

    unique_ptr<IoConnector> m_upIoConnector {};  
};