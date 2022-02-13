// PipeList.h
//
// NNetModel

#pragma once

#include <fstream>
#include <vector>

class Pipe;

using std::vector;
using std::unique_ptr;
using std::ranges::any_of;
using std::ranges::for_each;

using PipeFunc = function<void(Pipe &)>;
using PipeCrit = function<bool(Pipe const &)>;

class PipeList
{
public:

	unique_ptr<PipeList>Clone() const;

	void Dump() const;
	void Check() const;

	Pipe & GetFirst() { return * m_list.front(); }

	Pipe const & GetFirst() const { return * m_list.front(); }

	void Add(Pipe * const);
	void Add(PipeList const &);

	void Recalc() const;

	void Remove    (Pipe * const);
	bool TryRemove (Pipe * const);
	void Replace(Pipe * const, Pipe * const);

	bool IsEmpty   () const { return m_list.empty(); }
	bool IsNotEmpty() const { return ! IsEmpty(); }

	size_t Size() const { return m_list.size(); }

	void Clear() { m_list.clear(); }

	void Apply2All(auto const &f) const 
	{ 
		for_each(m_list, [&f](auto * const p) { f(*p); } );
	}

	bool Apply2AllB(auto const &f) const 
	{ 
		return any_of(m_list, [&f](auto const * p) { return f(*p); });
	}

private:
	vector<Pipe *> m_list {};
};

wostream & operator<<(wostream &, PipeList const &);
