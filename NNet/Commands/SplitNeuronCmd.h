// SplitNeuronCmd.h
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

class SplitNeuronCmd : public Command
{
public:
    SplitNeuronCmd
    (
        NNetModelWriterInterface & nmwi, 
        NobId                const id
   )
      : m_neuron(*nmwi.GetNobPtr<Neuron *>(id))
    {
        MicroMeterPnt umPos { m_neuron.GetPos() };
        m_upInputNeuron  = make_unique<InputNeuron >(umPos);
        m_upOutputNeuron = make_unique<OutputNeuron>(umPos);
        m_upInputNeuron ->SetOutgoing(m_neuron);
        m_upOutputNeuron->SetIncoming(m_neuron);
        m_upInputNeuron ->MoveNob((m_neuron.GetFirstOutgoing().GetEndPoint  ()-umPos).ScaledTo(NEURON_RADIUS*2));
        m_upOutputNeuron->MoveNob((m_neuron.GetFirstIncoming().GetStartPoint()-umPos).ScaledTo(NEURON_RADIUS*2));
    }

    ~SplitNeuronCmd() {}

    virtual void Do(NNetModelWriterInterface & nmwi)
    {
        m_upNeuron = nmwi.RemoveFromModel<Neuron>(m_neuron);
        nmwi.Push2Model(move(m_upInputNeuron));
        nmwi.Push2Model(move(m_upOutputNeuron));
    }

    virtual void Undo(NNetModelWriterInterface & nmwi)
    {
        m_upOutputNeuron = nmwi.PopFromModel<OutputNeuron>();
        m_upInputNeuron  = nmwi.PopFromModel<InputNeuron >();
        m_upNeuron->Reconnect();
        nmwi.Restore2Model<Neuron>(move(m_upNeuron));
    }

private:
    Neuron                 & m_neuron;
    unique_ptr<Neuron>       m_upNeuron       { };
    unique_ptr<InputNeuron>  m_upInputNeuron  { };
    unique_ptr<OutputNeuron> m_upOutputNeuron { };
};
