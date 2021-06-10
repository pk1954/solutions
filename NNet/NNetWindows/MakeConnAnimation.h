// MakeConnAnimation.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "win32_callable.h"
#include "win32_animation.h"
#include "Connector.h"
#include "NobPtrList.h"
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
        NNetModelWriterInterface & nmwi
    )
      : AnimationCmd(win),
        m_NMWI(nmwi)
    {
        m_pModelNobs = & m_NMWI.GetUPNobs();
        NobType const nobType { determineNobType(* m_pModelNobs) };
        if ( nobType.IsUndefinedType() )
            return;
        m_nobsAnimated = NobPtrList<IoNeuron>(m_pModelNobs->GetAllSelected<IoNeuron>(nobType));
        m_upConnector  = make_unique<Connector>( m_nobsAnimated );
    }

    virtual void Do(function<void()> const & targetReachedFunc)
    {
        m_upConnector->SetParentPointers();
        m_pModelNobs->Push(move(m_upConnector));
        (targetReachedFunc)();
    }

    virtual void Undo(function<void()> const & targetReachedFunc)
    {
        m_upConnector = m_pModelNobs->Pop<Connector>();
        m_upConnector->ClearParentPointers();
        (targetReachedFunc)();
    }

private:

    NobType const determineNobType(UPNobList const & list) const
    {
        unsigned int uiNrOfConnectors { list.CountInSelection( NobType::Value::connector ) };

        if ( uiNrOfConnectors > 0 )
            return NobType::Value::undefined;

        unsigned int uiNrOfInputNeurons  { list.CountInSelection( NobType::Value::inputNeuron  ) };
        unsigned int uiNrOfOutputNeurons { list.CountInSelection( NobType::Value::outputNeuron ) };

        if ((uiNrOfInputNeurons == 0) && (uiNrOfOutputNeurons == 0))
            return NobType::Value::undefined;

        return (uiNrOfInputNeurons > uiNrOfOutputNeurons) 
            ? NobType::Value::inputNeuron 
            : NobType::Value::outputNeuron;
    }

    UPNobList                * m_pModelNobs { nullptr };
    unique_ptr<Connector>      m_upConnector {};  
    NobPtrList<IoNeuron>       m_nobsAnimated {};
    NNetModelWriterInterface & m_NMWI;
};