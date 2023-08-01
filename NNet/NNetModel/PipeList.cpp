// PipeList.cpp
//
// NNetModel

module;

#include <cassert>
#include <iostream>
#include <algorithm>

module NNetModel:PipeList;

import IoUtil;
import :Pipe;

using std::endl;
using std::wcout;
using std::wostream;
using std::unique_ptr;
using std::make_unique;
using std::ranges::find;
using std::ranges::replace;

void PipeList::Dump() const
{
	wcout << OPEN_BRACKET;
	for (auto it : m_list) { wcout << it->GetId(); }
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

void PipeList::AddNNetPrefRapper(Pipe & pipe)
{ 
	if (find(m_list, &pipe) == end(m_list))
		m_list.push_back(&pipe); 
}

void PipeList::Remove(Pipe & pipe) 
{ 
	auto res { find(m_list, &pipe) };
	assert(res != end(m_list));
	m_list.erase(res);
}

void PipeList::Replace(Nob * const pDel, Nob * const pAdd) 
{ 
	assert(pAdd);
	assert(pDel);
	assert(pAdd->IsPipe());
	assert(pDel->IsPipe());
	replace(m_list, static_cast<Pipe *>(pDel), static_cast<Pipe*>(pAdd));
}

void PipeList::Recalc() const
{ 
	Apply2All([](Pipe & pipe) { pipe.PosChanged(); });
}

wostream & operator<<(wostream & out, PipeList const & pl)
{
	pl.Apply2All([&out](Pipe const & pipe) { out << pipe; });
	return out;
}
