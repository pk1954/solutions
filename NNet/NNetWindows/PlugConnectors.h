// PlugConnectors.h
//
// NNetWindows

#pragma once

#include <vector>
#include "NobId.h"
#include "NNetModelWriterInterface.h"
#include "ClosedConnector.h"
#include "Connector.h"
#include "AnimationCmd.h"

using std::vector;
using std::unique_ptr;

class MainWindow;

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
        m_size              = m_connectorAnimated.Size();
        for (size_t i = 0; i < m_size; ++i)
        {
            MicroMeterPnt const umPos    { m_connectorTarget.GetIoNeurons().GetElem(i).GetPos() };
            unique_ptr<Neuron>  upNeuron { make_unique<Neuron>(umPos) };
            upNeuron->SetIncoming(outputNeurons.GetElem(i));
            upNeuron->SetOutgoing(inputNeurons .GetElem(i));
            m_upClosedConnector->Push(upNeuron.get());
            m_upNeurons.push_back(move(upNeuron));
        }
        m_upIoNeuronsAnimated.resize(m_size);
        m_upIoNeuronsTarget  .resize(m_size);
        m_upClosedConnector->SetParentPointers();
    }

    virtual void Do(function<void()> const & targetReachedFunc)
    {
        for (size_t i = 0; i < m_size; ++i)
        {
            m_nmwi.Push2Model(move(m_upNeurons.back()));
            m_upNeurons.pop_back();
        }
        m_nmwi.Push2Model(move(m_upClosedConnector));

        m_upNobAnimated = m_nmwi.RemoveFromModel<Connector>(m_connectorAnimated);
        m_upNobTarget   = m_nmwi.RemoveFromModel<Connector>(m_connectorTarget  );
        for (size_t i = 0; i < m_size; ++i)
        {
            m_upIoNeuronsAnimated[i] = m_nmwi.RemoveFromModel<IoNeuron>(m_connectorAnimated.GetIoNeurons().GetElem(i));
            m_upIoNeuronsTarget  [i] = m_nmwi.RemoveFromModel<IoNeuron>(m_connectorTarget  .GetIoNeurons().GetElem(i));
        }

        if (targetReachedFunc)
            (targetReachedFunc)();
    }

    virtual void Undo(function<void()> const & targetReachedFunc)
    {
        m_upClosedConnector = m_nmwi.PopFromModel<ClosedConnector>();
        for (size_t i = 0; i < m_size; ++i)
            m_upNeurons.push_back(m_nmwi.PopFromModel<Neuron>());

        m_upNobAnimated = m_nmwi.ReplaceInModel<Connector, Connector>(move(m_upNobAnimated));
        m_upNobTarget   = m_nmwi.ReplaceInModel<Connector, Connector>(move(m_upNobTarget  ));
        for (size_t i = 0; i < m_size; ++i)
        {
            m_upIoNeuronsAnimated[i] = m_nmwi.ReplaceInModel<IoNeuron, IoNeuron>(move(m_upIoNeuronsAnimated[i]));
            m_upIoNeuronsTarget  [i] = m_nmwi.ReplaceInModel<IoNeuron, IoNeuron>(move(m_upIoNeuronsTarget  [i]));
        }

        if (targetReachedFunc)
            (targetReachedFunc)();
    }

private:

    NNetModelWriterInterface   & m_nmwi;
    size_t                       m_size;

    Connector                  & m_connectorAnimated;
    Connector                  & m_connectorTarget;

    // take ownership of ClosedConnector and Neurons between Undo and Redo
    unique_ptr<ClosedConnector>  m_upClosedConnector {};
    vector<unique_ptr<Neuron>>   m_upNeurons;              

    // take ownership of Connectors and IoNeurons between Do and Undo
    unique_ptr<Connector>        m_upNobAnimated;
    unique_ptr<Connector>        m_upNobTarget;
    vector<unique_ptr<IoNeuron>> m_upIoNeuronsAnimated;              
    vector<unique_ptr<IoNeuron>> m_upIoNeuronsTarget;              
};
