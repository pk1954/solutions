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

void add2copy(Nob   const &, UPNob);
void addMissingKnot(BaseKnot const &);

UPNobList CopySelectedNobs(NNetModelWriterInterface& nmwi)
{ 
	// create copy of selected nobs

	nmwi.GetUPNobs().Apply2AllSelected<Nob>
	(
		[&](Nob const & nob) { add2copy(nob, move(ShallowCopy(nob))); }
	); 	
	
	// m_nobs2Add has contiguous NobIds
	// links are still pointing to model nobs
	
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

	m_nobs2Add.Apply2All  // fix links
	(
		[&](Nob & nob) { nob.Link(copy2model(&nob), model2copy); }
	);

	m_nobs2Add.CheckNobList();
	m_nobs2Add.Move(PIPE_WIDTH);  // dislocate copy
	return move(m_nobs2Add);
}

//////  local functions /////////////////////////////////

void add2copy(Nob const & nobModel, UPNob upNobCopy)
{
	m_mapModel2copy.insert(make_pair(&nobModel, upNobCopy.get()));
	m_mapCopy2model.insert(make_pair(upNobCopy.get(), &nobModel));
	m_nobs2Add.Push(move(upNobCopy));
}

void addMissingKnot(BaseKnot const & baseKnotModel)
{
	Nob * pNobCopy { model2copy(&baseKnotModel) };
	if (!pNobCopy)
		add2copy(baseKnotModel, move(make_unique<Knot>(baseKnotModel.GetPos())));
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
	return (it != m_mapModel2copy.end()) ?  it->second : (Nob *)0;
}
