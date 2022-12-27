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
import :Synapse;
import :PosNob;
import :Knot;
import :Nob;

using std::unordered_map;
using std::unique_ptr;
using std::make_unique;
using std::pair;
using std::move;

unique_ptr<UPNobList> CopySelectedNobs::Do(NNetModelWriterInterface & nmwi)
{ 
	unique_ptr<UPNobList> m_upNobs2Add { make_unique<UPNobList>() };

	m_mapCopy2model.clear();
	m_mapModel2copy.clear();

	//nmwi.GetUPNobs().Apply2AllSelected<Nob>
	//(
	//	[](Nob& n) { n.SelectAllConnected(true); }
	//);

	nmwi.GetUPNobs().Apply2AllSelected<Nob>  // create copy of selected nobs
	(
		[&m_upNobs2Add](Nob const & nobModel)
		{ 
			UPNob upNobCopy { ShallowCopy(nobModel) };
			m_mapModel2copy.insert(pair(&nobModel, upNobCopy.get()));
			m_mapCopy2model.insert(pair(upNobCopy.get(), &nobModel));
			m_upNobs2Add->Push(move(upNobCopy));
		}
	); 	

	// m_upNobs2Add has contiguous NobIds. Links are still pointing to model nobs.
	
	m_upNobs2Add->Apply2AllC   // fix links
	(
		[](Nob & nobDst) 
		{ 
			nobDst.Link(copy2model(&nobDst), model2copy); 
		}
	);

	m_upNobs2Add->Move(MicroMeterPnt(PIPE_WIDTH, PIPE_WIDTH));  // dislocate copy

	return move(m_upNobs2Add);
}

//////  local functions /////////////////////////////////


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
