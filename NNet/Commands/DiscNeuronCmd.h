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
    DiscNeuronCmd(NNetModelWriterInterface & nmwi, NobId const idNeuron)
        : m_idNeuron(idNeuron)
    {
        m_pNeuron = nmwi.GetNobPtr<Neuron *>( m_idNeuron );
        if ( ! m_pNeuron )   // might have been deleted earlier
            return;

        MicroMeterPnt umPos { m_pNeuron->GetPos() };
        
        m_upInputNeuron  = make_unique<InputNeuron>(umPos);
        m_upOutputNeuron = make_unique<OutputNeuron>(umPos);
        m_upInputNeuron ->m_connections.SetOutgoing(m_pNeuron->m_connections);
        m_upOutputNeuron->m_connections.SetIncoming(m_pNeuron->m_connections);
        m_upInputNeuron ->m_connections.GetFirstOutgoing().DislocateStartPoint();
        m_upOutputNeuron->m_connections.GetFirstIncoming().DislocateEndPoint  ();
    }

    ~DiscNeuronCmd() {}

    virtual void Do(NNetModelWriterInterface & nmwi)
    {
        if ( ! m_pNeuron )   // might have been deleted earlier
            return;

        nmwi.Push2Model(move(m_upInputNeuron ));
        nmwi.Push2Model(move(m_upOutputNeuron));
        m_upNeuron       = nmwi.RemoveFromModel<Neuron>(*m_pNeuron);
        nmwi.CheckModel();
    }

    virtual void Undo(NNetModelWriterInterface & nmwi)
    {
        if ( ! m_pNeuron )   // might have been deleted earlier
            return;

        m_upOutputNeuron = nmwi.PopFromModel<OutputNeuron>();
        m_upInputNeuron  = nmwi.PopFromModel<InputNeuron >();
        m_upNeuron->Reconnect();
        nmwi.GetUPNobs().SetNob2Slot( move(m_upNeuron) );
        nmwi.CheckModel();
    }

private:
    NobId              const m_idNeuron;
    Neuron                 * m_pNeuron { nullptr };
    unique_ptr<Neuron>       m_upNeuron       { };
    unique_ptr<InputNeuron>  m_upInputNeuron  { };
    unique_ptr<OutputNeuron> m_upOutputNeuron { };
};
