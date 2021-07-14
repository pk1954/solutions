// DiscNeuronCmd.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "Command.h"
#include "NobId.h"
#include "Neuron.h"
#include "InputNeuron.h"
#include "OutputNeuron.h"

using std::unique_ptr;

class DiscNeuronCmd : public Command
{
public:
    DiscNeuronCmd
    (
        NNetModelWriterInterface & nmwi, 
        Nob                      & nob
    )
      : m_neuron(*Cast2Neuron(&nob))
    {
        MicroMeterPnt umPos { m_neuron.GetPos() };
        Connections & conn  { m_neuron.m_connections };
        m_upInputNeuron  = make_unique<InputNeuron >(umPos);
        m_upOutputNeuron = make_unique<OutputNeuron>(umPos);
        m_upInputNeuron ->m_connections.SetOutgoing(conn);
        m_upOutputNeuron->m_connections.SetIncoming(conn);
        m_upInputNeuron ->MoveNob((conn.GetFirstOutgoing().GetEndPoint  ()-umPos).ScaledTo(NEURON_RADIUS*2));
        m_upOutputNeuron->MoveNob((conn.GetFirstIncoming().GetStartPoint()-umPos).ScaledTo(NEURON_RADIUS*2));
    }

    ~DiscNeuronCmd() {}

    virtual void Do(NNetModelWriterInterface & nmwi)
    {
        nmwi.Push2Model(move(m_upInputNeuron ));
        nmwi.Push2Model(move(m_upOutputNeuron));
        m_upNeuron = nmwi.RemoveFromModel<Neuron>(m_neuron);
    }

    virtual void Undo(NNetModelWriterInterface & nmwi)
    {
        m_upOutputNeuron = nmwi.PopFromModel<OutputNeuron>();
        m_upInputNeuron  = nmwi.PopFromModel<InputNeuron >();
        m_upNeuron->Reconnect();
        nmwi.Restore2Model(move(m_upNeuron));
    }

private:
    Neuron                 & m_neuron;
    unique_ptr<Neuron>       m_upNeuron       { };
    unique_ptr<InputNeuron>  m_upInputNeuron  { };
    unique_ptr<OutputNeuron> m_upOutputNeuron { };
};
