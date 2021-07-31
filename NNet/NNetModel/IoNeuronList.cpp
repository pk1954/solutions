// IoNeuronList.cpp 
//
// NNetModel

#include "stdafx.h"
#include <algorithm>
#include "IoNeuron.h"
#include "IoNeuronList.h"

using std::sort;
using std::wcout;
using std::endl;
using std::reverse;

void IoNeuronList::Check() const 
{ 
	if (m_list.empty())
		return;
	NobType const nobType { GetFirst().GetNobType() };
	for (auto & it : m_list) 
	{ 
		assert(it->GetNobType() == nobType);
		it->Check();
	}; 
}

void IoNeuronList::Dump() const
{
	wcout << *this;
}

void IoNeuronList::Reverse()
{
	reverse(m_list.begin(), m_list.end());
}

MicroMeterPnt const IoNeuronList::GetPos() const 
{ 
	return (GetFirst().GetPos() + GetLast().GetPos()) * 0.5f; 
}

void IoNeuronList::Replace(IoNeuron * const pDel, IoNeuron * const pAdd) 
{ 
	replace(begin(m_list), end(m_list), pDel, pAdd); 
}

void IoNeuronList::Add(IoNeuron * const pNob)
{
	if ( pNob != nullptr )
	{
		assert(find(begin(m_list), end(m_list), pNob) == end(m_list));
		m_list.push_back( pNob );
	}
}

void IoNeuronList::Remove(IoNeuron * const pNob)
{
	auto res = find(begin(m_list), end(m_list), pNob);
	assert( res != end(m_list) );
	m_list.erase( res );
}

void IoNeuronList::Apply2All(function<void(IoNeuron &)> const & func) const
{
	for ( auto pNob : m_list ) 
		if (pNob)
			func( * pNob );
}

bool const IoNeuronList::Apply2AllB(function<bool(IoNeuron const &)> const & func) const 
{
	bool bResult { false };
	for ( auto pNob : m_list ) 
		if (pNob && func(*pNob))
			return true;
	return false;
}

size_t const IoNeuronList::Count(NobType const nobType) const 
{
	size_t counter { 0 };
	for (auto pNob : m_list)
		if (pNob && (pNob->GetNobType() == nobType))
			++counter;
	return counter;
}

void IoNeuronList::SelectAll(bool const bOn) 
{ 
	for (auto it: m_list)
		it->Select(bOn);
}

bool IoNeuronList::operator==(IoNeuronList const & rhs) const
{
	if ( Size() != rhs.Size() )
		return false;
	for ( int i = 0; i < Size(); ++i )
		if ( m_list[i] != rhs.m_list[i] )
			return false;
	return true;
}

wostream & operator<< (wostream & out, IoNeuronList const & v)
{
	out << IoNeuronList::OPEN_BRACKET << v.m_list.size() << IoNeuronList::NR_SEPARATOR;
	for ( auto & it: v.m_list )
	{
		out << it->GetId();
		if ( &it == &v.m_list.back() )
			break;
		out << IoNeuronList::ID_SEPARATOR;
	}
	out << IoNeuronList::CLOSE_BRACKET;
	return out;
}

void IoNeuronList::SortAccToDistFromLine(MicroMeterLine const & line)
{
	sort
	( 
		m_list.begin(), m_list.end(),
		[&](auto & p1, auto & p2) 
		{ 
			return PointToLine(line, p1->GetPos()) < PointToLine(line, p2->GetPos()); 
		} 
	);
}

void IoNeuronList::AlignDirection()
{
	MicroMeterLine const umLine(GetFirst().GetPos(), GetLast().GetPos());
	MicroMeterPnt  const umPntDir { CalcOrthoVector(umLine) };
	for (auto it : m_list) { it->SetDirVector(umPntDir); }
}

void IoNeuronList::Link(Nob2NobFunc const & dstFromSrc)
{
	for (int i = 0; i < m_list.size(); ++i)
		m_list[i] = static_cast<IoNeuron *>(dstFromSrc(m_list[i]));
}

bool const IoNeuronList::IsIncludedIn(MicroMeterRect const & umRect) const 
{
	for (auto it : m_list) { if (it->IsIncludedIn(umRect)) return true; }
	return false;
}

bool const IoNeuronList::Includes(MicroMeterPnt const & umPnt) const
{
	for (auto it : m_list) { if (it->Includes(umPnt)) return true; }
	return false;
}

void IoNeuronList::DrawExterior(DrawContext const & context, tHighlight const type) const
{
	for (auto it : m_list) { it->DrawExterior(context, type); }
}

void IoNeuronList::DrawInterior(DrawContext const & context, tHighlight const type) const
{
	for (auto it : m_list) { it->DrawInterior(context, type); }
}

void IoNeuronList::Expand(MicroMeterRect & umRect) const
{
	for (auto it : m_list) { umRect.Expand(it->GetPos()); }
}

Radian const IoNeuronList::GetDir() const 
{ 
	return IsEmpty() ? Radian::NULL_VAL() : GetFirst().GetDir();
}

void IoNeuronList::RotateNobs(MicroMeterPnt const & umPntPivot, Radian const radDelta)
{
	for (auto it : m_list) { it->RotateNob(umPntPivot, radDelta); }
}

void IoNeuronList::MoveNobs(MicroMeterPnt const & delta)       
{
	for (auto it : m_list) { it->MoveNob(delta); }
}

void IoNeuronList::Prepare()
{
	for (auto it : m_list) { it->Prepare(); }
}

bool const IoNeuronList::CompStep()
{
	for (auto it : m_list) { if (it->CompStep()) return true; }
	return false;
}

void IoNeuronList::Recalc()
{
	for (auto it : m_list) { it->Recalc(); }
}

void IoNeuronList::SetParentPointers(Nob * const pNob)
{
	for (auto it : m_list) { it->SetParentNob(pNob); }
}

void IoNeuronList::ClearParentPointers()
{
	for (auto it : m_list) { it->SetParentNob(nullptr); }
}
