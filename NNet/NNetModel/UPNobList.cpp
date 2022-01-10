// UPNobList.cpp 
//
// NNetModel

#include "stdafx.h"
#include <numeric>
#include "Knot.h"
#include "Pipe.h"
#include "BaseKnot.h"
#include "Neuron.h"
#include "InputConnector.h"
#include "OutputConnector.h"
#include "InputNeuron.h"
#include "OutputNeuron.h"
#include "NobIdList.h"
#include "NobException.h"
#include "UPNobList.h"

using std::move;
using std::accumulate;

void UPNobList::Clear()
{ 
	m_list.clear(); 
	m_nobsOfType.fill(0);
}

UPNob ShallowCopy(Nob const & nob)  //TODO: simplify!  
{
	NobType::Value type { nob.GetNobType().GetValue() };
	switch (type)
	{
	using enum NobType::Value;
	case inputConnector:  return Copy<InputConnector>(nob);
	case outputConnector: return Copy<OutputConnector>(nob);
	case inputNeuron:	  return Copy<InputNeuron>(nob);
	case outputNeuron:	  return Copy<OutputNeuron>(nob);
	case knot:		      return Copy<Knot>(nob);
	case neuron:		  return Copy<Neuron>(nob);
	case pipe:		      return Copy<Pipe>(nob);

	default:
		ThrowNobTypeException(nob);
		return nullptr;
	}
}

bool UPNobList::operator==(UPNobList const & other) const
{
	if (m_list.size() != other.m_list.size())
		return false;
	for (int i = 0; i < m_list.size(); ++i)
	{
		Nob const * pNob      {       m_list[i].get() };
		Nob const * pNobOther { other.m_list[i].get() };
		if ((pNob == nullptr) != (pNobOther == nullptr))
		{
			return false;
		}
		// Now either both ptrs are null or none
		if ((pNob != nullptr) && (*pNob != *pNobOther))
		{
			return false;
		}
	}
	return true;
}

UPNob UPNobList::ExtractNob(NobId const id)	
{
	assert(IsDefined(id));
	assert(IsValidNobId(id));

	UPNob upNob { move(m_list[id.GetValue()]) };
	if (upNob)
		decCounter(*upNob.get());
	return move(upNob);
}

void UPNobList::SetNob2Slot(UPNob upNob)
{                 
	NobId const id { upNob->GetId() };
	SetNob2Slot(id, move(upNob));
}

void UPNobList::SetNob2Slot(NobId const id, UPNob upNob) 
{
	assert(IsDefined(id));
	assert(IsValidNobId(id));
	assert(IsEmptySlot(id));
	assert(upNob);

	incCounter(*upNob.get());
	m_list[id.GetValue()] = move(upNob);
}

Nob * UPNobList::ReplaceNob(UPNob upT)
{
	NobId const id { upT->GetId() };

	assert(IsDefined(id));
	assert(IsValidNobId(id));

	incCounter(*upT.get());
	decCounter(*GetAt(id));

	UPNob tmp = move(upT);
	m_list[id.GetValue()].swap(tmp);
	return tmp.release();
}

NobId UPNobList::Push(UPNob upNob)	
{
	NobId idNewSlot { IdNewSlot() };
	if (upNob)
	{
		upNob->SetId(idNewSlot);
		incCounter(*upNob.get());
	}
	m_list.push_back(move(upNob));
	return idNewSlot;
}

void UPNobList::MoveFrom(UPNobList & src, size_t nrOfItems)
{
	for (size_t i = 0; i < nrOfItems; ++i)
		Push(src.Pop());
}

void UPNobList::copy(const UPNobList & rhs)
{
	rhs.CheckNobList();

	m_list.resize(rhs.m_list.size());

	for (auto const & pNobSrc : rhs.m_list)
	{
		if (pNobSrc)
			SetNob2Slot(pNobSrc->GetId(), ShallowCopy(*pNobSrc));
	}

	for (auto const & pNobDst : m_list)
	{
		if (pNobDst)
			pNobDst->Link
			(
				*rhs.GetAt(pNobDst->GetId()),                           // the src nob used as template for links
				[this](Nob const *pSrc){ return GetAt(pSrc->GetId()); } // how to get dst nob from src nob
		    );
	}

	CheckNobList();
}

UPNobList::~UPNobList()
{
	Clear();
}

UPNobList::UPNobList(const UPNobList & rhs) // copy constructor
{
	copy(rhs);
}

UPNobList & UPNobList::operator= (const UPNobList & rhs) // copy assignment
{
	if (this != &rhs)
	{
		Clear();
		copy(rhs);
	}
	return * this;
}

void UPNobList::CheckNobList() const
{
#ifdef _DEBUG
	for (size_t i = 0; i < m_list.size(); ++i)
	{
		if (m_list[i])
		{
			assert(m_list[i]->GetId().GetValue() == i);
			m_list[i]->Check();
		}
	}
#endif
}

void UPNobList::Dump() const
{
	for (auto const & it : m_list)
		if (it)
			it->Dump();
}

MicroMeterRect UPNobList::CalcEnclosingRect(SelMode const mode) const
{
	MicroMeterRect rect { MicroMeterRect::ZERO_VAL() };
	for (auto const & it : m_list)
		if (it && ((mode == SelMode::allNobs) || it->IsSelected()))
			it->Expand(rect);
	return rect;
}

unique_ptr<vector<Nob *>> UPNobList::GetAllSelected()
{
	auto upNobs { make_unique<vector<Nob*>>() };
	for (auto const & it : m_list)
	{
		if (it && it->IsSelected()) 
			upNobs->push_back(it.get());
	}
	return move(upNobs);
}

void UPNobList::SelectAllNobs(bool const bOn) const
{ 
	for (auto const & it : m_list)
		if (it)
			it->Select(bOn);
}

void UPNobList::Move(MicroMeterPnt const& delta) const
{
	for (auto const & it : m_list)
		if (it)
			it->MoveNob(delta);
}

unsigned int UPNobList::CountInSelection(NobType const nobType) const
{
	unsigned int uiNr { 0 };
	Apply2AllSelected(nobType, [&uiNr](auto &) { ++uiNr; });
	return uiNr;
}

unsigned int UPNobList::GetCounter(NobType const t) const 
{ 
	return m_nobsOfType[static_cast<unsigned int>(t.GetValue())]; 
}

unsigned int UPNobList::GetCounter() const 
{ 
	return accumulate(m_nobsOfType.begin(), m_nobsOfType.end(), 0); 
}

void UPNobList::countNobs()
{
	m_nobsOfType.fill(0);
	for (auto const & it : m_list)
		if (it)
		{ 
			NobType    const type  { it->GetNobType() };
			unsigned int const index { static_cast<unsigned int>(type.GetValue()) };
			++m_nobsOfType[index];
		};
}

bool UPNobList::Contains(Nob const * pNob) const 
{ 
	return Apply2AllB([pNob](Nob const & nob) { return pNob == &nob; }); 
}

void UPNobList::Reconnect(NobId const id) const
{
	if (Nob * pNod { GetAt(id) })
		pNod->Reconnect();
}
