// CopySelectedNobs.cpp
//
// NNetModel

module;

#include <cassert>
#include <unordered_map>
#include "Nob.h"
#include "BaseKnot.h"
#include "NNetModelWriterInterface.h"

module CopySelectedNobs;
import Knot;

using std::unordered_map;
using std::make_pair;

UPNobList CopySelectedNobs::Do(NNetModelWriterInterface & nmwi)
{ 
	m_mapCopy2model.clear();
	m_mapModel2copy.clear();
	m_nobs2Add.Clear();

	nmwi.GetUPNobs().Apply2AllSelected<Nob>  // create copy of selected nobs
	(
		[](Nob const & nobModel) { add2copy(nobModel, ShallowCopy(nobModel)); }
	); 	
	
	// m_nobs2Add has contiguous NobIds
	// links are still pointing to model nobs
	
	// add Knots to pipe endpoints missing in copy

	for (int i = 0; i < m_nobs2Add.Size(); ++i)      // cannot use range-based loop.
	{                                                   // m_nobs2Add modified in loop.
		Nob * pNobCopy { m_nobs2Add.GetAt(NobId(i)) };
		if (pNobCopy->IsPipe())
		{
			Pipe const & pipeModel { static_cast<Pipe const &>(copy2model(pNobCopy)) };
			addMissingKnot(*(pipeModel.GetStartKnotPtr()));
			addMissingKnot(*(pipeModel.GetEndKnotPtr  ()));
		}
	}

	m_nobs2Add.Apply2AllC  // fix links
	(
		[](Nob & nobDst) { nobDst.Link(copy2model(&nobDst), model2copy); }
	);

	m_nobs2Add.Move(MicroMeterPnt(PIPE_WIDTH, PIPE_WIDTH));  // dislocate copy

	return m_nobs2Add;
}

//////  local functions /////////////////////////////////

void CopySelectedNobs::add2copy(Nob const & nobModel, UPNob upNobCopy)
{
	Nob * const pNobCopy { upNobCopy.get() };
	assert(pNobCopy);
	m_mapModel2copy.insert(make_pair(&nobModel, pNobCopy));
	m_mapCopy2model.insert(make_pair(pNobCopy, &nobModel));
	m_nobs2Add.Push(move(upNobCopy));
}

void CopySelectedNobs::addMissingKnot(BaseKnot const & baseKnotModel)
{
	if (!model2copy(&baseKnotModel))
		add2copy(baseKnotModel, make_unique<Knot>(baseKnotModel.GetPos()));
}

Nob const & CopySelectedNobs::copy2model(Nob * const pNobCopy) 
{ 
	assert(pNobCopy);
	assert(IsDefined(pNobCopy->GetId()));
	unordered_map<Nob *, Nob const *>::const_iterator const it { m_mapCopy2model.find(pNobCopy) };
	assert(it != m_mapCopy2model.end());
	return * it->second;
}

Nob * CopySelectedNobs::model2copy(Nob const * const pNobModel) 
{ 
	assert(pNobModel);
	assert(IsDefined(pNobModel->GetId()));
	unordered_map<Nob const *, Nob *>::const_iterator const it { m_mapModel2copy.find(pNobModel) };
	return (it != m_mapModel2copy.end()) ? it->second : nullptr;
}
