// UPNobList.cpp 
//
// NNetModel

module;

#include <cassert>
#include <numeric>
#include <vector>
#include <memory>

module NNetModel:UPNobList;

import :NobException;
import :InputConnector;
import :OutputConnector;
import :InputLine;
import :OutputLine;
import :Synapse;
import :Fork;
import :Knot;
import :Neuron;
import :Pipe;
import :Nob;
import :NobId;

using std::move;
using std::vector;
using std::unique_ptr;
using std::make_unique;
using std::accumulate;

void UPNobList::Clear()
{ 
	m_list.clear(); 
	m_nobsOfType.fill(0);
}

UPNob ShallowCopy(Nob const & nob)
{
	NobType::Value type { nob.GetNobType().GetValue() };
	switch (type)
	{
	using enum NobType::Value;
	case inputConnector:  return make_unique<InputConnector >(static_cast<InputConnector  const&>(nob));
	case outputConnector: return make_unique<OutputConnector>(static_cast<OutputConnector const&>(nob));
	case inputLine:   	  return make_unique<InputLine      >(static_cast<InputLine       const&>(nob));
	case outputLine:	  return make_unique<OutputLine     >(static_cast<OutputLine      const&>(nob));
	case synapse:		  return make_unique<Synapse        >(static_cast<Synapse         const&>(nob));
	case fork:		      return make_unique<Fork           >(static_cast<Fork            const&>(nob));
	case knot:		      return make_unique<Knot           >(static_cast<Knot            const&>(nob));
	case neuron:		  return make_unique<Neuron         >(static_cast<Neuron          const&>(nob));
	case pipe:		      return make_unique<Pipe           >(static_cast<Pipe            const&>(nob));

	default:
		throw NobTypeException(nob.GetNobType(), __FILE__, __LINE__);
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
	decCounter(upNob.get());
	return move(upNob);
}

void UPNobList::SetNob2Slot(UPNob upNob)
{                 
	NobId const id { upNob->GetId() };
	SetNob2Slot(id, move(upNob));
}

void UPNobList::SetNob2Slot(NobId const id, UPNob upNob) 
{
	assert(upNob);
	assert(IsDefined(id));
	assert(IsValidNobId(id));
	if (IsEmptySlot(id))
	{
		incCounter(upNob.get());
		m_list[id.GetValue()] = move(upNob);
	}
	else
	{
		ReplaceNob(move(upNob));
	}
}

Nob * UPNobList::ReplaceNob(UPNob upT)
{
	NobId const id { upT->GetId() };

	assert(IsDefined(id));
	assert(IsValidNobId(id));

	incCounter(upT.get());
	decCounter(GetAt(id));

	UPNob tmp = move(upT);
	m_list[id.GetValue()].swap(tmp);
	return tmp.release();
}

NobId UPNobList::Push(UPNob upNob)	
{
	NobId const idNewSlot { NobId(Size()) };
	if (upNob)
	{
		upNob->SetId(idNewSlot);
		incCounter(upNob.get());
	}
	m_list.push_back(move(upNob));
	return idNewSlot;
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
