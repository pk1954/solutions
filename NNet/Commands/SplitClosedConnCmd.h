// SplitClosedConnCmd.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "Command.h"
#include "NobId.h"
#include "NobIdList.h"
#include "UPNobList.h"
#include "ClosedConnector.h"

using std::unique_ptr;

class SplitClosedConnCmd : public Command
{
public:
    SplitClosedConnCmd
    (
        NNetModelWriterInterface & nmwi,
        NobId                const idClosedConnector
    )
        : m_idClosedConnector(idClosedConnector)
    {
        ClosedConnector const & closedConnector { * nmwi.GetNobPtr<ClosedConnector *>(idClosedConnector) };
        m_inputNeurons  = closedConnector.GetInputConnector ().GetIoNeurons();
        m_outputNeurons = closedConnector.GetOutputConnector().GetIoNeurons();
        m_nrOfNeurons   = closedConnector.Size();
        for ( size_t i = 0; i < m_nrOfNeurons; ++i )
        {
            IoNeuron         & inputNeuron  { m_inputNeurons .GetElem(i) };
            IoNeuron         & outputNeuron { m_outputNeurons.GetElem(i) };
            MicroMeterPnt      umPntPos     { inputNeuron.GetPos() };
            unique_ptr<Neuron> upNeuron     { make_unique<Neuron>(umPntPos, inputNeuron, outputNeuron) };
            m_neuronIds.Add(m_upNeuronList.Push(move(upNeuron)));  // Ownership of neurons
        }
    }

    ~SplitClosedConnCmd() {}

    virtual void Do( NNetModelWriterInterface & nmwi )
    {
        m_upClosedConnector = nmwi.RemoveFromModel<ClosedConnector>(m_idClosedConnector); // Take ownership of ClosedConnector
        for ( size_t i = 0; i < m_nrOfNeurons; ++i )
        {                                                        // Take ownership of IoNeurons
            m_upInputNeurons .Push(nmwi.RemoveFromModel<IoNeuron>(m_inputNeurons .GetElem(i)));
            m_upOutputNeurons.Push(nmwi.RemoveFromModel<IoNeuron>(m_outputNeurons.GetElem(i)));
            nmwi.Add2Model(move(m_upNeuronList.Pop<Neuron>()));  // Move ownership of Neurons to model
        }
    }

    virtual void Undo( NNetModelWriterInterface & nmwi )
    {
        nmwi.GetUPNobs().SetNob2Slot(move(m_upClosedConnector));             // Move ownership of ClosedConnector to model
        for ( size_t i = 0; i < m_nrOfNeurons; ++i )
        {                                                     
            nmwi.GetUPNobs().SetNob2Slot(m_upInputNeurons .Pop<IoNeuron>()); // Move ownership of IoNeurons to model
            nmwi.GetUPNobs().SetNob2Slot(m_upOutputNeurons.Pop<IoNeuron>());
            m_upNeuronList.Push(nmwi.RemoveFromModel<Neuron>(m_neuronIds.Get(i)));   // Take ownership of Neurons
        }
    }

private:

    size_t                      m_nrOfNeurons;
    NobId                const  m_idClosedConnector;
    unique_ptr<ClosedConnector> m_upClosedConnector {};
    NobPtrList<IoNeuron>        m_inputNeurons      {}; // original input neurons in ClosedConnector
    NobPtrList<IoNeuron>        m_outputNeurons     {}; // original output neurons in ClosedConnector 
    UPNobList                   m_upInputNeurons    {}; // store input neurons between Do and Undo
    UPNobList                   m_upOutputNeurons   {}; // store output neurons between Do and Undo
    UPNobList                   m_upNeuronList      {}; // store neurons between Undo and Redo 
    NobIdList                   m_neuronIds         {}; // store Neuron ids when neurons in model 
};
