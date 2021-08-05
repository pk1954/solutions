// PipeList.h
//
// NNetModel

#pragma once

#include <fstream>
#include <vector>

class Pipe;

using std::vector;
using std::unique_ptr;

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

	void Add(Pipe   * const   p) { if (p) m_list.push_back(p); }
	void Add(PipeList const & l) { l.Apply2All([&](Pipe &p) { Add(&p); }); }

	void Recalc();

	void Remove (Pipe * const);
	void Replace(Pipe * const, Pipe * const);

	bool const IsEmpty   () const { return m_list.empty(); }
	bool const IsNotEmpty() const { return ! IsEmpty(); }

	size_t const Size() const { return m_list.size(); }

	void Clear() { m_list.clear(); }

	void Apply2All (PipeFunc const &) const;
	bool Apply2AllB(PipeCrit const &) const; 

private:
	vector<Pipe *> m_list {};
};

wostream & operator<<(wostream &, PipeList const &);
