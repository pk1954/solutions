// PlugIoConnectors.h
//
// Commands

#pragma once

#include <vector>
#include "NobId.h"
#include "NNetModelWriterInterface.h"
#include "IoConnector.h"
#include "NNetCommand.h"

using std::vector;
using std::unique_ptr;

class MainWindow;

class PlugIoConnectors : public NNetCommand
{
public:
    PlugIoConnectors
    (
        IoConnector & conn1, 
        IoConnector & conn2
    )
      : m_inputConnector (conn1.IsInputNob () ? conn1 : conn2),
        m_outputConnector(conn1.IsOutputNob() ? conn1 : conn2)
    {
        assert(m_inputConnector.Size() == m_outputConnector.Size());

        m_size = m_inputConnector.Size();
        for (size_t i = 0; i < m_size; ++i)
        {
            MicroMeterPnt const umPos    { m_inputConnector.GetElem(i).GetPos() };
            unique_ptr<Neuron>  upNeuron { make_unique<Neuron>(umPos) };
            upNeuron->SetIncoming(m_outputConnector.GetElem(i));
            upNeuron->SetOutgoing(m_inputConnector .GetElem(i));
            m_upNeurons.push_back(move(upNeuron));
        }
        m_upOutputNeurons.resize(m_size);
        m_upInputNeurons .resize(m_size);
        m_pNMWI->CheckModel();
    }

    void Do() final
    {
        for (size_t i = 0; i < m_size; ++i)
        {
            m_pNMWI->Push2Model(move(m_upNeurons.back()));
            m_upNeurons.pop_back();
        }

        m_upOutputConnector = m_pNMWI->RemoveFromModel<IoConnector>(m_outputConnector);
        m_upInputConnector  = m_pNMWI->RemoveFromModel<IoConnector>(m_inputConnector );
        for (size_t i = 0; i < m_size; ++i)
        {
            m_upOutputNeurons[i] = m_pNMWI->RemoveFromModel<IoNeuron>(m_outputConnector.GetElem(i));
            m_upInputNeurons [i] = m_pNMWI->RemoveFromModel<IoNeuron>(m_inputConnector .GetElem(i));
        }
        (m_targetReachedFunc)();
    }

    virtual void Undo()
    {
        for (size_t i = 0; i < m_size; ++i)
            m_upNeurons.push_back(m_pNMWI->PopFromModel<Neuron>());

        m_upOutputConnector = m_pNMWI->ReplaceInModel<IoConnector, IoConnector>(move(m_upOutputConnector));
        m_upInputConnector  = m_pNMWI->ReplaceInModel<IoConnector, IoConnector>(move(m_upInputConnector ));
        for (size_t i = 0; i < m_size; ++i)
        {
            m_upOutputNeurons[i] = m_pNMWI->ReplaceInModel<IoNeuron, IoNeuron>(move(m_upOutputNeurons[i]));
            m_upInputNeurons [i] = m_pNMWI->ReplaceInModel<IoNeuron, IoNeuron>(move(m_upInputNeurons [i]));
        }
        (m_targetReachedFunc)();
    }

private:

    size_t                       m_size;

    IoConnector          const & m_inputConnector;
    IoConnector          const & m_outputConnector;

    vector<unique_ptr<Neuron>>   m_upNeurons;              

    // take ownership of IoConnectors and IoNeurons between Do and Undo
    unique_ptr<IoConnector>      m_upInputConnector;
    unique_ptr<IoConnector>      m_upOutputConnector;
    vector<unique_ptr<IoNeuron>> m_upInputNeurons;              
    vector<unique_ptr<IoNeuron>> m_upOutputNeurons;              
};
