// PipeList.cpp
//
// NNetModel

module;

#include <cassert>
#include <iostream>
#include <algorithm>

module NNetModel:PipeList;

import IoConstants;
import :Pipe;

using std::endl;
using std::wcout;
using std::wostream;
using std::unique_ptr;
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
	wcout << OPEN_BRACKET;
	for (auto it : m_list) { wcout << it->GetId() << L' '; }
	wcout << CLOSE_BRACKET;
}

void PipeList::Check() const
{
	for (auto & it : m_list)
	{
		assert(it);
		it->Check();
	}
}

void PipeList::Add(Pipe & pipe) 
{ 
	if (find(m_list, &pipe) == end(m_list))
		m_list.push_back(&pipe); 
}

void PipeList::Add(PipeList const & l) 
{ 
	l.Apply2All([this](Pipe & p) { Add(p); }); 
}

bool PipeList::TryRemove(Pipe & pipe) 
{ 
	auto res  { find(m_list, &pipe) };
	bool bRes { res != end(m_list) };
	if (bRes)
		m_list.erase(res);
	return bRes;
}

void PipeList::Remove(Pipe & pipe) 
{ 
	bool bRes { TryRemove(pipe) };
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
