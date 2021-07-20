// PlugConnectors.h
//
// NNetCommands

#pragma once

#include <vector>
#include "NobId.h"
#include "NNetModelWriterInterface.h"
#include "ClosedConnector.h"
#include "Connector.h"
#include "AnimationCmd.h"

using std::vector;
using std::unique_ptr;

class PlugConnectors : public AnimationCmd
{
public:
    PlugConnectors
    (
        NNetModelWriterInterface & nmwi,
        Connector                & connectorAnimated, 
        Connector                & connectorTarget,
        MainWindow               & win
    )
      : AnimationCmd(win),
        m_nmwi(nmwi),
        m_connectorTarget(connectorTarget),
        m_connectorAnimated(connectorAnimated)
    {
        assert( m_connectorAnimated.IsCompositeNob() == m_connectorTarget.IsCompositeNob() );
        assert( m_connectorAnimated.GetIoMode() != NobIoMode::internal );
        assert( m_connectorTarget  .GetIoMode() != NobIoMode::internal );
        assert( m_connectorTarget  .GetIoMode() != m_connectorAnimated.GetIoMode() );
        assert( m_connectorAnimated.Size() == m_connectorTarget.Size() );

        Connector    const & inputConnector  { m_connectorAnimated.IsInputNob () ? m_connectorAnimated : m_connectorTarget };
        Connector    const & outputConnector { m_connectorAnimated.IsOutputNob() ? m_connectorAnimated : m_connectorTarget };
        IoNeuronList const & inputNeurons    { inputConnector .GetIoNeurons() };
        IoNeuronList const & outputNeurons   { outputConnector.GetIoNeurons() };
        m_upClosedConnector = make_unique<ClosedConnector>();
        for (size_t i = 0; i < m_connectorAnimated.Size(); ++i)
        {
            MicroMeterPnt const umPos { m_connectorTarget.GetPos() };
            unique_ptr<Neuron> upNeuron { make_unique<Neuron>(umPos) };
            upNeuron->SetIncoming(inputNeurons .GetElem(i));
            upNeuron->SetOutgoing(outputNeurons.GetElem(i));
            m_upNeurons.push_back(move(upNeuron));
        }
    }

    virtual void Do(function<void()> const & targetReachedFunc)
    {
        m_upClosedConnector->SetParentPointers();
        m_nmwi.Push2Model(move(m_upClosedConnector)); 
        m_upNobAnimated = m_nmwi.RemoveFromModel<Connector>(m_connectorAnimated);
        m_upNobTarget   = m_nmwi.RemoveFromModel<Connector>(m_connectorTarget);
        if (targetReachedFunc)
            (targetReachedFunc)();
    }

    virtual void Undo(function<void()> const & targetReachedFunc)
    {
        m_upClosedConnector = m_nmwi.PopFromModel<ClosedConnector>();
        m_upClosedConnector->ClearParentPointers();
        m_upNobAnimated->Reconnect();
        m_upNobTarget  ->Reconnect();
        m_upNobAnimated = m_nmwi.ReplaceInModel<Connector,Connector>(move(m_upNobAnimated));
        m_upNobTarget   = m_nmwi.ReplaceInModel<Connector,Connector>(move(m_upNobTarget));
        m_nmwi.GetUPNobs().DeselectAllNobs();
        if (targetReachedFunc)
            (targetReachedFunc)();
    }

private:
    NNetModelWriterInterface  & m_nmwi;
    Connector                 & m_connectorTarget;
    Connector                 & m_connectorAnimated;
    unique_ptr<Connector>       m_upNobAnimated;
    unique_ptr<Connector>       m_upNobTarget;
    unique_ptr<ClosedConnector> m_upClosedConnector {};
    vector<unique_ptr<Neuron>>  m_upNeurons;              
};
