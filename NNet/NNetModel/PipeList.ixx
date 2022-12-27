// PipeList.ixx
//
// NNetModel

module;

#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>

export module NNetModel:PipeList;

import :Pipe;

using std::vector;
using std::unique_ptr;
using std::wostream;
using std::ranges::any_of;
using std::ranges::for_each;

export class PipeList
{
public:

	void Dump() const;
	void Check() const;

	Pipe       & Get(size_t const i)       { return * m_list.at(i); }
	Pipe const & Get(size_t const i) const { return * m_list.at(i); }

	void Set(size_t const i, Pipe* p) { m_list[i] = p; }

	Pipe       & GetFirst()       { return * m_list.front(); }
	Pipe const & GetFirst() const { return * m_list.front(); }

	void Recalc() const;

	void Add(Pipe &);
	void Remove(Pipe &);
	void Replace(Nob * const, Nob * const);

	bool IsEmpty   () const { return m_list.empty(); }
	bool IsNotEmpty() const { return !IsEmpty(); }

	size_t Size() const { return m_list.size(); }
	void Resize(size_t const n) { m_list.resize(n); }

	void Clear() { m_list.clear(); }

	void Apply2All(auto const& f) const
	{
		for_each(m_list, [&f](auto* const p) { f(*p); });
	}

	bool Apply2AllB(auto const& f) const
	{
		return any_of(m_list, [&f](auto const* p) { return f(*p); });
	}

	void SelectAllConnected(bool const bOn)
	{
		for_each(m_list, [bOn](auto* const p) { p->SelectAllConnected(false, bOn); });
	}

private:
	vector<Pipe*> m_list{};
};

export wostream & operator<<(wostream &, PipeList const &);
