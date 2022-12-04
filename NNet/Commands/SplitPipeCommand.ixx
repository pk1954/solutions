// SplitPipeCommand.ixx
//
// Commands

module;

#include <memory>

export module SplitPipeCommand;

import Types;
import NNetCommand;
import NNetModel;

using std::unique_ptr;
using std::make_unique;
using std::pair;

export class SplitPipeCommand : public NNetCommand
{
public:
	SplitPipeCommand
	(
		NobId         const  idPipe,
		MicroMeterPnt const& umSplitPoint
	)
	  :	m_idPipe2Split(idPipe),
		m_pPipe2Split (m_pNMWI->GetNobPtr<Pipe*>(idPipe)),
		m_fPosSplit   (m_pPipe2Split->PosOnPipe(umSplitPoint)),
		m_pNobStart   (Cast2PosNob(m_pPipe2Split->GetStartNobPtr())),
		m_pNobEnd     (Cast2PosNob(m_pPipe2Split->GetEndNobPtr()))
	{
	}

	void InitSplit(PosNob & posNobSplit)
	{
		m_splitPipes = m_pPipe2Split->Split(posNobSplit);
	}

	~SplitPipeCommand() final = default;

	void Do() override
	{
		m_pNobStart->ReplaceOutgoing(m_pPipe2Split, m_splitPipes.first.get());
		m_pNobEnd  ->ReplaceIncoming(m_pPipe2Split, m_splitPipes.second.get());
		m_pPipe2Split->Apply2AllSynapses
		(
			[this](Nob* pNob)
			{
				Pipe* pPipeNew { SelectPipe(pNob, m_splitPipes, m_fPosSplit) };
				Cast2Synapse(pNob)->SetMainPipe(pPipeNew);
			}
		);
		m_pNMWI->Push2Model(move(m_splitPipes.first));
		m_pNMWI->Push2Model(move(m_splitPipes.second));
		m_upPipe2Split = m_pNMWI->RemoveFromModel<Pipe>(m_idPipe2Split);
	}

	void Undo() override
	{
		m_pNMWI->Restore2Model(move(m_upPipe2Split));
		m_splitPipes.second = m_pNMWI->PopFromModel<Pipe>();
		m_splitPipes.first  = m_pNMWI->PopFromModel<Pipe>();
		m_splitPipes.first ->Apply2AllSynapses([this](Nob* pNob) { Cast2Synapse(pNob)->SetMainPipe(m_pPipe2Split); });
		m_splitPipes.second->Apply2AllSynapses([this](Nob* pNob) { Cast2Synapse(pNob)->SetMainPipe(m_pPipe2Split); });
		m_pNobEnd  ->ReplaceIncoming(m_splitPipes.second.get(), m_pPipe2Split);
		m_pNobStart->ReplaceOutgoing(m_splitPipes.first.get(), m_pPipe2Split);
	}

private:

	NobId      const m_idPipe2Split;
	Pipe     * const m_pPipe2Split;
	float      const m_fPosSplit;
	PosNob   * const m_pNobStart;
	PosNob   * const m_pNobEnd;
	PipePair         m_splitPipes;
	unique_ptr<Pipe> m_upPipe2Split;
};
