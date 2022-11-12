// CopySelectedNobs.cpp
//
// NNetModel

module;

#include <cassert>
#include <memory>
#include <unordered_map>

module NNetModel:CopySelectedNobs;

import Types;
import :NNetModelWriterInterface;
import :UPNobList;
import :PosNob;
import :Knot;
import :Nob;

using std::unordered_map;
using std::make_unique;
using std::pair;

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
			addMissingKnot(*(static_cast<PosNob const *>(pipeModel.GetStartNobPtr())));
			addMissingKnot(*(static_cast<PosNob const *>(pipeModel.GetEndNobPtr  ())));
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
	m_mapModel2copy.insert(pair(&nobModel, pNobCopy));
	m_mapCopy2model.insert(pair(pNobCopy, &nobModel));
	m_nobs2Add.Push(move(upNobCopy));
}

void CopySelectedNobs::addMissingKnot(PosNob const & posNobModel)
{
	if (!model2copy(&posNobModel))
		add2copy(posNobModel, make_unique<Knot>(posNobModel.GetPos()));
}

Nob const & CopySelectedNobs::copy2model(Nob * const pNobCopy) 
{ 
	assert(pNobCopy);
	assert(IsDefined(pNobCopy->GetId()));
	auto const it { m_mapCopy2model.find(pNobCopy) };
	assert(it != m_mapCopy2model.end());
	return * it->second;
}

Nob * CopySelectedNobs::model2copy(Nob const * const pNobModel) 
{ 
	assert(pNobModel);
	assert(IsDefined(pNobModel->GetId()));
	auto const it { m_mapModel2copy.find(pNobModel) };
	return (it != m_mapModel2copy.end()) ? it->second : nullptr;
}
