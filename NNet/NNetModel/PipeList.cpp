// PipeList.cpp
//
// NNetModel

#include "stdafx.h"
#include "SCANNER.H"
#include "Pipe.h"
#include "PipeList.h"

using std::endl;
using std::wcout;
using std::make_unique;
using std::ranges::find;
using std::ranges::replace;

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

void PipeList::Add(Pipe * const p) 
{ 
	if (p && (find(m_list, p) == end(m_list)))
		m_list.push_back(p); 
}

void PipeList::Add(PipeList const & l) 
{ 
	l.Apply2All([this](Pipe &p) { Add(&p); }); 
}

bool PipeList::TryRemove(Pipe * const p) 
{ 
	auto res  { find(m_list, p) };
	bool bRes { res != end(m_list) };
	if (bRes)
		m_list.erase(res);
	return bRes;
}

void PipeList::Remove(Pipe * const p) 
{ 
	bool bRes { TryRemove(p) };
	assert(bRes);
}

void PipeList::Replace(Pipe * const pDel, Pipe * const pAdd) 
{ 
	assert(pAdd);
	replace(m_list, pDel, pAdd); 
}

void PipeList::Recalc() const
{ 
	Apply2All([](Pipe & pipe) { pipe.Recalc(); }); 
}

wostream & operator<<(wostream & out, PipeList const & pl)
{
	pl.Apply2All([&out](Pipe const & pipe) { out << pipe; });
	return out;
}
