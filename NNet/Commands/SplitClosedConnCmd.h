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
        closedConnector.Check();
        NobPtrList<IoNeuron> const & inputNeurons  { closedConnector.GetInputConnector ().GetIoNeurons() };
        NobPtrList<IoNeuron> const & outputNeurons { closedConnector.GetOutputConnector().GetIoNeurons() };
        m_nrOfNeurons = closedConnector.Size();
        for ( size_t i = 0; i < m_nrOfNeurons; ++i )
        {
            unique_ptr<Neuron> upNeuron { make_unique<Neuron>(inputNeurons.GetElem(i).GetPos(), inputNeurons.GetElem(i), outputNeurons.GetElem(i)) };
            m_upNeuronList.push_back(move(upNeuron));  // Ownership of neurons
        }
    }

    ~SplitClosedConnCmd() {}

    virtual void Do( NNetModelWriterInterface & nmwi )
    {
        m_upClosedConnector = nmwi.RemoveFromModel<ClosedConnector>(m_idClosedConnector); // Take ownership of ClosedConnector
        NobPtrList<IoNeuron> const & inputNeurons  { m_upClosedConnector->GetInputConnector ().GetIoNeurons() };
        NobPtrList<IoNeuron> const & outputNeurons { m_upClosedConnector->GetOutputConnector().GetIoNeurons() };
        for ( size_t i = 0; i < m_nrOfNeurons; ++i )
        {                                                        // Take ownership of IoNeurons
            m_upInputNeurons .push_back(nmwi.RemoveFromModel<IoNeuron>(inputNeurons .GetElem(i)));
            m_upOutputNeurons.push_back(nmwi.RemoveFromModel<IoNeuron>(outputNeurons.GetElem(i)));
            nmwi.Push2Model(move(m_upNeuronList.back()));        // Move ownership of Neurons to model
            m_upNeuronList.pop_back();
        }
        nmwi.DumpModel();
    }

    virtual void Undo( NNetModelWriterInterface & nmwi )
    { 
        nmwi.ReplaceInModel<ClosedConnector,ClosedConnector>(move(m_upClosedConnector)); // Move ownership of ClosedConnector to model
        for ( size_t i = 0; i < m_nrOfNeurons; ++i )
        {                                                     
            nmwi.ReplaceInModel<IoNeuron,IoNeuron>(move(m_upInputNeurons .back()));      // Move ownership of IoNeurons to model
            nmwi.ReplaceInModel<IoNeuron,IoNeuron>(move(m_upOutputNeurons.back()));
            m_upInputNeurons .pop_back();
            m_upOutputNeurons.pop_back();
            m_upNeuronList.push_back(nmwi.PopFromModel<Neuron>());                       // Take ownership of Neurons
        }
        nmwi.DumpModel();
    }

private:

    size_t                      m_nrOfNeurons;
    NobId                const  m_idClosedConnector;
    unique_ptr<ClosedConnector> m_upClosedConnector {};

    vector<unique_ptr<IoNeuron>> m_upInputNeurons   {}; // store input neurons between Do and Undo
    vector<unique_ptr<IoNeuron>> m_upOutputNeurons  {}; // store output neurons between Do and Undo
    vector<unique_ptr<Neuron>>   m_upNeuronList     {}; // store neurons between Undo and Redo 
};
