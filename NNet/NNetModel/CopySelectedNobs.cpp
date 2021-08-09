// CopySelectionCommand.cpp
//
// Commands

#include "stdafx.h"
#include <unordered_map>
#include "Nob.h"
#include "Knot.h"
#include "BaseKnot.h"
#include "NamedType.h"
#include "NNetModelWriterInterface.h"
#include "CopySelectedNobs.h"

#include "IoConnector.h"

using std::vector;
using std::unordered_map;
using std::unique_ptr;
using std::make_unique;
using std::make_pair;

unordered_map<Nob const *, Nob       *> m_mapModel2copy {}; // maps model to copy
unordered_map<Nob       *, Nob const *> m_mapCopy2model {}; // maps copy to model

size_t    m_nrOfNobs { 0 };
UPNobList m_nobs2Add {};

Nob * const model2copy(Nob   const * const);
Nob const & copy2model(Nob * const); 

void add2copy(Nob const &, UPNob);
void addMissingKnot(BaseKnot const &);

void checkPipe(Nob const & nob, UPNobList const & list)
{
	Pipe const & pipe { static_cast<Pipe const &>(nob) };
	assert(list.Contains(pipe.GetStartKnotPtr())); 
	assert(list.Contains(pipe.GetEndKnotPtr  ())); 
}

void checkNob(Nob const & nob, UPNobList const & list)
{
	assert(list.Contains(&nob)); 
	switch (nob.GetNobType().GetValue())
	{
	case NobType::Value::inputConnector:
	case NobType::Value::outputConnector:
	case NobType::Value::closedConnector:
	{
		IoConnector const & con { static_cast<IoConnector const &>(nob) };
		con.Apply2All
		(
			[&](Neuron & neuron) 
			{ 
				assert(list.Contains(&neuron)); 
			}
		);
	}
	break;

	case NobType::Value::inputNeuron:
	case NobType::Value::outputNeuron:
	case NobType::Value::knot:
	case NobType::Value::neuron:
	{
		BaseKnot const & baseKnot { static_cast<BaseKnot const &>(nob) };
		baseKnot.Apply2AllConnectedPipes
		(
			[&](Pipe & pipe) 
			{ 
				assert(list.Contains(&pipe)); 
			}
		);
	}
	break;

	case NobType::Value::pipe:
		checkPipe(nob, list);
		break;

	default:
		assert(false);
		break;
	}
}

UPNobList CopySelectedNobs(NNetModelWriterInterface & nmwi)
{ 
	nmwi.CheckModel();
	// create copy of selected nobs

	m_mapCopy2model.clear();
	m_mapModel2copy.clear();
	m_nobs2Add.Clear();
	nmwi.GetUPNobs().Apply2AllSelected<Nob>
	(
		[&](Nob const & nobModel) 
		{ 
			UPNob upNobCopy { move(ShallowCopy(nobModel)) };
//			add2copy(nob, move(upNobCopy)); 
			Nob * const pNobCopy { upNobCopy.get() };
			m_mapModel2copy.insert(make_pair(&nobModel, pNobCopy));
			
			
			if ( m_mapCopy2model.find( pNobCopy ) != m_mapCopy2model.end( ) )
			{
				int x = 42;
			}
			m_mapCopy2model.insert(make_pair(pNobCopy, &nobModel));


			m_nobs2Add.Push(move(upNobCopy));
			assert(m_nobs2Add.Contains(pNobCopy)); 
		}
	); 	
	
	// m_nobs2Add has contiguous NobIds
	// links are still pointing to model nobs
	
	for (auto const & it : m_mapModel2copy)
	{
		Nob const * pNobModel = it.first;
		Nob const * pNobCopy  = it.second;
		assert(nmwi.GetUPNobs().Contains(pNobModel)); 
		assert(m_nobs2Add      .Contains(pNobCopy)); 
	}

	for (auto const & it : m_mapCopy2model)
	{
		Nob const * pNobCopy  = it.first;
		Nob const * pNobModel = it.second;
		assert(nmwi.GetUPNobs().Contains(pNobModel)); 
		assert(m_nobs2Add      .Contains(pNobCopy)); 
	}

	// add Knots to pipe endpoints missing in copy

	for (size_t i = 0; i < m_nobs2Add.Size(); ++i)      // cannot use range-based loop.
	{                                                   // m_nobs2Add modified in loop.
		Nob * pNobCopy { m_nobs2Add.GetAt(NobId(i)) };
		if (pNobCopy->IsPipe())
		{
			Pipe const & pipeModel { static_cast<Pipe const &>(copy2model(pNobCopy)) };
			addMissingKnot(*(pipeModel.GetStartKnotPtr()));
			addMissingKnot(*(pipeModel.GetEndKnotPtr  ()));
		}
	}

	for (auto const & it : m_mapModel2copy)
	{
		Nob const * pNobModel = it.first;
		Nob const * pNobCopy  = it.second;
		assert(pNobCopy);
		assert(nmwi.GetUPNobs().Contains(pNobModel)); 
		assert(m_nobs2Add      .Contains(pNobCopy));
	}

	for (auto const & it : m_mapCopy2model)
	{
		Nob const * pNobCopy  = it.first;
		Nob const * pNobModel = it.second;
		assert(nmwi.GetUPNobs().Contains(pNobModel)); 
		assert(m_nobs2Add      .Contains(pNobCopy)); 
	}

	m_nobs2Add.Apply2All  // fix links
	(
		[&](Nob & nobDst) 
		{ 
			UPNobList const & list = m_nobs2Add;
			//assert(m_nobs2Add.Contains(&nobDst)); 
			//assert(list      .Contains(&nobDst)); 
			//assert(m_mapModel2copy[m_mapCopy2model[&nobDst]] == &nobDst);
//			nob.Link(copy2model(&nob), model2copy); 
			switch (nobDst.GetNobType().GetValue())
			{
			case NobType::Value::inputConnector:
			case NobType::Value::outputConnector:
			case NobType::Value::closedConnector:
			{
				IoConnector & conDst { static_cast<IoConnector &>(nobDst) };
				conDst.Link(copy2model(&nobDst), model2copy); 
				conDst.Apply2All
				(
					[&](Neuron & neuronDst) 
					{ 
						assert(list.Contains(&neuronDst)); 
					}
				);
			}
			break;

			case NobType::Value::inputNeuron:
			case NobType::Value::outputNeuron:
			case NobType::Value::knot:
			case NobType::Value::neuron:
			{
				BaseKnot       & baseKnotDst { static_cast<BaseKnot &>(nobDst) };
	//			BaseKnot const & baseKnotSrc { static_cast<BaseKnot const &>(copy2model(&nobDst)) };
	//			baseKnotDst.ClearConnections();
				baseKnotDst.Link(copy2model(&nobDst), model2copy); 

				//baseKnotSrc.Apply2AllOutPipes
				//(
				//	[&](Pipe const & pipeSrc)
				//	{
				//		//if (pipeSrc.IsSelected())
				//		//{
				//			Pipe * pPipeDst { static_cast<Pipe *>(model2copy(&pipeSrc)) };
				//			baseKnotDst.AddOutgoing(static_cast<Pipe *>(pPipeDst));
				//		//}
				//		//else
				//		//{
				//		//	Pipe * pPipeDst { static_cast<Pipe *>(model2copy(&pipeSrc)) };
				//		//	baseKnotDst.AddOutgoing(static_cast<Pipe *>(pPipeDst));
				//		//}
				//	}
				//);
				//baseKnotSrc.Apply2AllInPipes
				//(
				//	[&](Pipe const & pipeSrc)
				//	{
				//		//if (pipeSrc.IsSelected())
				//		//{
				//			Pipe * pPipeDst { static_cast<Pipe *>(model2copy(&pipeSrc)) };
				//			baseKnotDst.AddIncoming(static_cast<Pipe *>(pPipeDst));
				//		//}
				//		//else
				//		//{
				//		//	Pipe * pPipeDst { static_cast<Pipe *>(model2copy(&pipeSrc)) };
				//		//	baseKnotDst.AddIncoming(static_cast<Pipe *>(pPipeDst));
				//		//}
				//	}
				//);
				//if (baseKnotSrc.GetParentNob())
				//	baseKnotDst.SetParentNob(model2copy(baseKnotSrc.GetParentNob()));

			}
			break;

			case NobType::Value::pipe:
			{
				Pipe & pipeDst { static_cast<Pipe &>(nobDst) };
				pipeDst.Link(copy2model(&nobDst), model2copy); 
			}
				break;

			default:
				assert(false);
				break;
			}
			checkNob(nobDst, m_nobs2Add); 
		}
	);

	m_nobs2Add.Apply2All
	( 
		[&](Nob & nob) 
		{ 
			checkNob(nob, m_nobs2Add); 
		} 
	); 

	m_nobs2Add.Move(PIPE_WIDTH);  // dislocate copy
	return move(m_nobs2Add);
}

//////  local functions /////////////////////////////////

void add2copy(Nob const & nobModel, UPNob upNobCopy)
{
	Nob * const pNobCopy { upNobCopy.get() };
	assert(pNobCopy);
	m_mapModel2copy.insert(make_pair(&nobModel, pNobCopy));
	m_mapCopy2model.insert(make_pair(pNobCopy, &nobModel));
	m_nobs2Add.Push(move(upNobCopy));
	assert(m_nobs2Add.Contains(pNobCopy)); 
}

void addMissingKnot(BaseKnot const & baseKnotModel)
{
	Nob * pNobCopy { model2copy(&baseKnotModel) };
	if (!pNobCopy)
		add2copy(baseKnotModel, move(make_unique<Knot>(baseKnotModel.GetPos())));
	else
	{
		int x = 42;
	}
}

Nob const & copy2model(Nob * const pNobCopy) 
{ 
	assert(pNobCopy);
	assert(IsDefined(pNobCopy->GetId()));
	unordered_map<Nob *, Nob const *>::const_iterator const it { m_mapCopy2model.find(pNobCopy) };
	assert(it != m_mapCopy2model.end());
	return * it->second;
}

Nob * const model2copy(Nob const * const pNobModel) 
{ 
	assert(pNobModel);
	assert(IsDefined(pNobModel->GetId()));
	unordered_map<Nob const *, Nob *>::const_iterator const it { m_mapModel2copy.find(pNobModel) };
	if (it != m_mapModel2copy.end()) 
	{
		return it->second;
	}
	else 
		return (Nob *)0;
}
