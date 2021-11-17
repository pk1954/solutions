// PipeList.cpp
//
// NNetModel

#include "stdafx.h"
#include "Scanner.h"
#include "Pipe.h"
#include "PipeList.h"

using std::endl;
using std::wcout;
using std::make_unique;

unique_ptr<PipeList>PipeList::Clone() const 
{
	unique_ptr<PipeList> upCopy { make_unique<PipeList>() };
	upCopy->m_list = m_list;
	return move(upCopy);
}

void PipeList::Dump() const
{
	wcout << Pipe::OPEN_BRACKET;
	for (auto it : m_list) { wcout << it->GetId() << L' '; }
	wcout << Pipe::CLOSE_BRACKET;
}

void PipeList::Check() const
{
	for (auto & it : m_list)
	{
		assert(it);
		it->Check();
	}
}

void PipeList::Remove(Pipe * const p) 
{ 
	auto res = find(begin(m_list), end(m_list), p);
	assert(res != end(m_list));
	m_list.erase(res);
}

void PipeList::Replace(Pipe * const pDel, Pipe * const pAdd) 
{ 
	assert(pAdd);
	replace(m_list.begin(), m_list.end(), pDel, pAdd); 
}

void PipeList::Apply2All(PipeFunc const &f) const 
{ 
	for (auto & it : m_list) 
		f(* it); 
}

bool PipeList::Apply2AllB(PipeCrit const &f) const 
{ 
	for (auto & it : m_list) 
	{ 
		if (f(* it))
			return true;
	}
	return false;
}

void PipeList::Recalc() const
{ 
	Apply2All([&](Pipe & pipe) { pipe.Recalc(); }); 
}

wostream & operator<<(wostream & out, PipeList const & pl)
{
	pl.Apply2All([&](Pipe & pipe) { out << pipe; });
	return out;
}
