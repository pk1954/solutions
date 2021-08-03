// CopySelectionCommand.cpp
//
// Commands

#include "stdafx.h"
#include "Knot.h"
#include "BaseKnot.h"
#include "NNetModelWriterInterface.h"
#include "CopySelectionCommand.h"

CopySelectionCommand::CopySelectionCommand(NNetModelWriterInterface & nmwi)
{ 
	auto dstFromSrc = [&](Nob const * pSrc)
	{ 
		if (pSrc && IsDefined(pSrc->GetId()))
		{
			SelNobsIndex & ssi { m_indexList[pSrc->GetId().GetValue()] };
			if (ssi.IsNotNull())
				return m_copies[ssi.GetValue()].get();
		}
		return (Nob *)0;
	};

	size_t const sizeOfModel { nmwi.GetUPNobs().Size() };
	NobId        idNobCopy   { NobId(sizeOfModel) };

	m_indexList.resize(sizeOfModel, SelNobsIndex::NULL_VAL()); 

	nmwi.GetUPNobs().Apply2AllSelected<Nob>
	(
		[&](Nob & nob)
		{
			UPNob upNobCopy { ShallowCopy(nob) };
			m_indexList[upNobCopy->GetId().GetValue()] = SelNobsIndex(m_copies.size());
			m_copies.push_back(move(upNobCopy));
		}
	);
	size_t const iSize { m_copies.size() };
	for (size_t i = 0; i < iSize; ++i) // cannot use range-based loop. m_copies changed in loop.
	{
		Nob * const pNobSrc { nmwi.GetNob(m_copies[i]->GetId()) };
		if (pNobSrc->IsPipe())
		{
			Pipe * const pPipeSrc { static_cast<Pipe *>(pNobSrc) };
			addMissingKnot(pPipeSrc->GetStartKnotPtr(), dstFromSrc);
			addMissingKnot(pPipeSrc->GetEndKnotPtr  (), dstFromSrc);
		}
	}
	for (UPNob & upNobDst : m_copies)  // link nobs
	{
		Nob const & nobSrc { * nmwi.GetNob(upNobDst->GetId()) };
		nmwi.GetUPNobs().LinkNob(nobSrc, dstFromSrc);
		upNobDst->SetId(idNobCopy++);
		if (upNobDst->GetNobType().IsBaseKnotType())
			upNobDst->MoveNob(PIPE_WIDTH); 
	}
	m_sizeOfSelection = m_copies.size();
}

void CopySelectionCommand::Do(NNetModelWriterInterface & nmwi) 
{ 
	SelectionCommand::Do(nmwi);
	nmwi.GetUPNobs().DeselectAllNobs();  
	for (size_t i = 0; i < m_sizeOfSelection; ++i)
	{
		nmwi.Push2Model(move(m_copies.back())); // add copies (which are already selected and linked)
		m_copies.pop_back();
	}
	assert(m_copies.empty());
}

void CopySelectionCommand::Undo(NNetModelWriterInterface & nmwi) 
{ 
	for (size_t i = 0; i < m_sizeOfSelection; ++i)
	{
		m_copies.push_back(nmwi.PopFromModel<Nob>());
	}
	SelectionCommand::Undo(nmwi);
}

void CopySelectionCommand::addMissingKnot(BaseKnot * pBaseKnot, Nob2NobFunc const & dstFromSrc)
{
	if (!dstFromSrc(pBaseKnot))
	{
		unique_ptr<Knot> upKnot { make_unique<Knot>(pBaseKnot->GetPos()) };
		upKnot->SetId(pBaseKnot->GetId());
		m_indexList[upKnot->GetId().GetValue()] = SelNobsIndex(Cast2Int(m_copies.size()));
		m_copies.push_back(move(upKnot));
	}
}
