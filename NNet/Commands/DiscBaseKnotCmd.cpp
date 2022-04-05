// DeleteBaseKnotCmd.cpp
//
// NNetModel

#include "stdafx.h"
#include "DeleteBaseKnotCmd.h"

using std::wcout;
using std::endl;
using std::make_unique;

DeleteBaseKnotCmd::DeleteBaseKnotCmd(Nob & nob)
  : m_baseKnot(*Cast2BaseKnot(&nob))
{
    m_idOutputNeurons.Resize(m_baseKnot.GetNrOfInConns());
    m_idInputNeurons .Resize(m_baseKnot.GetNrOfOutConns());

    m_umPos = m_baseKnot.GetPos();
    m_baseKnot.Apply2AllOutPipes
    (
        [this](Pipe & pipe) // every outgoing pipe needs a new InputNeuron as terminator
        { 
            auto upInputNeuron { make_unique<InputNeuron>(m_pNMWI->StdSigGen(), m_umPos) };
            upInputNeuron->Select(pipe.IsSelected());
            upInputNeuron->AddOutgoing(pipe);            // prepare new InputNeuron as far as possible
            m_inputNeurons.push_back(move(upInputNeuron)); // store new InputNeuron for later
        }                                                  // but do not touch m_pBaseKnot
    );  // InputNeuron in m_inputNeurons have their outgoing pipe set
    m_baseKnot.Apply2AllInPipes
    (
        [this](Pipe & pipe) // every incoming pipe needs a new OutputNeuron as terminator
        { 
            auto upOutputNeuron { make_unique<OutputNeuron>(m_umPos) };
            upOutputNeuron->Select(pipe.IsSelected());
            upOutputNeuron->AddIncoming(pipe);             // prepare new OutputNeuron as far as possible
            m_outputNeurons.push_back(move(upOutputNeuron)); // store new OutputNeuron for later
        }                                                    // but do not touch m_pBaseKnot
    );  // OutputNeuron in m_outputNeurons have their incoming pipe set
}

void DeleteBaseKnotCmd::Do()
{
    for (size_t i = 0; i < m_outputNeurons.size(); ++i)
    {
        unique_ptr<OutputNeuron> & upOutputNeuron { m_outputNeurons[i] };
        if (upOutputNeuron.get())
        {
            Pipe & pipeOut { upOutputNeuron->GetFirstIncoming() };
            pipeOut.SetEndKnot(upOutputNeuron.get());
            pipeOut.DislocateEndPoint();
            m_idOutputNeurons.SetAt(i, m_pNMWI->Push2Model(move(upOutputNeuron)));
        }
    }
    for (size_t i = 0; i < m_inputNeurons.size(); ++i)
    {
        unique_ptr<InputNeuron> & upInputNeuron { m_inputNeurons[i] };
        if (upInputNeuron.get())
        {
            Pipe & pipeIn { upInputNeuron->GetFirstOutgoing() };
            pipeIn.SetStartKnot(upInputNeuron.get());
            pipeIn.DislocateStartPoint();
            m_idInputNeurons.SetAt(i, m_pNMWI->Push2Model(move(upInputNeuron)));
        }
    }
    m_upBaseKnot = m_pNMWI->RemoveFromModel<BaseKnot>(m_baseKnot);
}

void DeleteBaseKnotCmd::Undo()
{
    for (size_t i = m_inputNeurons.size(); i --> 0;)
    {
        if (auto pInputNeuron { m_pNMWI->GetNobPtr<InputNeuron *>(m_idInputNeurons.Get(i)) })
            ConnectOutgoing(pInputNeuron->GetFirstOutgoing(), m_baseKnot);
        m_inputNeurons[i] = m_pNMWI->PopFromModel<InputNeuron>();
    }
    for (size_t i = m_outputNeurons.size(); i --> 0;)
    {
        if (auto pOutputNeuron { m_pNMWI->GetNobPtr<OutputNeuron *>(m_idOutputNeurons.Get(i)) })
            ConnectIncoming(pOutputNeuron->GetFirstIncoming(), m_baseKnot);
        m_outputNeurons[i] = m_pNMWI->PopFromModel<OutputNeuron>();
    }
    m_upBaseKnot = m_pNMWI->ReplaceInModel<BaseKnot>(move(m_upBaseKnot));
}
