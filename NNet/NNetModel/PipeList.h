// PipeList.h
//
// NNetModel

#pragma once

#include <fstream>
#include <vector>
#include "scanner.h"
#include "Pipe.h"

using std::endl;
using std::wcout;
using std::vector;
using std::unique_ptr;
using std::make_unique;

using PipeFunc = function<void(Pipe &)>;
using PipeCrit = function<bool(Pipe const &)>;

class PipeList
{
public:

	unique_ptr<PipeList>Clone() const 
	{
		unique_ptr<PipeList> upCopy { make_unique<PipeList>() };
		upCopy->m_list = m_list;
		return move(upCopy);
	}

	void Dump() const
	{
		wcout << Pipe::OPEN_BRACKET;
		for (auto it : m_list) { wcout << it->GetId() << L' '; }
		wcout << Pipe::CLOSE_BRACKET;
	}

	Pipe & GetFirst() { return * m_list.front(); }

	Pipe const & GetFirst() const { return * m_list.front(); }

	void Add(Pipe   * const   p) { m_list.push_back(p); }
	void Add(PipeList const & l) { l.Apply2All([&](Pipe &p) { Add(&p); }); }

	void Remove(Pipe * const p) 
	{ 
		auto res = find(begin(m_list), end(m_list), p);
		assert( res != end(m_list) );
		m_list.erase( res );
	}

	void Replace(Pipe * const pDel, Pipe * const pAdd) 
	{ 
		replace(m_list.begin(), m_list.end(), pDel, pAdd); 
	}

	bool const IsEmpty   () const { return m_list.empty(); }
	bool const IsNotEmpty() const { return ! IsEmpty(); }

	size_t const Size() const { return m_list.size(); }

	void Clear() { m_list.clear(); }

	void Apply2All(PipeFunc const &f) const 
	{ 
		for (auto it : m_list) 
		{ 
			f(* it); 
		} 
	}

	bool Apply2AllB(PipeCrit const &f) const 
	{ 
		for (auto it : m_list) 
		{ 
			if (f(* it))
				return true;
		}
		return false;
	}

	void Recalc()
	{
		Apply2All([&](Pipe & pipe) { pipe.Recalc(); } );
	}

	friend wostream & operator<<(wostream & out, PipeList const & pl)
	{
		for (auto it : pl.m_list) { out << * it; }
		return out;
	}

private:
	vector<Pipe *> m_list {};
};
