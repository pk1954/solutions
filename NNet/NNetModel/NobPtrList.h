// NobPtrList.h
//
// NNetModel

#pragma once

#include <vector>
#include "MoreTypes.h"
#include "Nob.h"

using std::vector;
using std::sort;

template <Nob_t T>
class NobPtrList
{
public:

	NobPtrList() {}
	virtual ~NobPtrList() {}

	size_t const Size()    const { return m_list.size(); }
	bool   const IsEmpty() const { return m_list.empty(); }

	T       & GetFirst()       { return * m_list.at( 0 ); }
	T const & GetFirst() const { return * m_list.at( 0 ); }

	T       & GetLast()       { return * m_list.back(); }
	T const & GetLast() const { return * m_list.back(); }

	void Check() const { for (auto & it : m_list) { it->Check(); }; }

	void Clear()      {	m_list.clear(); }
	void RemoveLast() {	m_list.pop_back(); }
	void Replace(T * const pDel, T * const pAdd) 
	{ 
		replace( begin(m_list), end(m_list), pDel, pAdd ); 
	}

	void Add( T * const pNob )
	{
		if ( pNob != nullptr )
		{
			assert( find( begin(m_list), end(m_list), pNob ) == end(m_list) );
			m_list.push_back( pNob );
		}
	}

	void Remove( T * const pNob )
	{
		auto res = find( begin(m_list), end(m_list), pNob );
		assert( res != end(m_list) );
		m_list.erase( res );
	}

	void Apply2All( function<void(T &)> const & func ) const
	{
		for ( auto pNob : m_list ) 
		{ 
			if ( pNob != nullptr )
				func( * pNob );
		}
	}

	bool Apply2AllB( function<bool(T const &)> const & func ) const 
	{
		bool bResult { false };
		for ( auto pNob : m_list ) 
		{ 
			if ( pNob != nullptr )
			{
				if ( func( * pNob ) )
					return true;
			}
		}
		return false;
	}

	bool operator==(NobPtrList const & rhs) const
	{
		if ( Size() != rhs.Size() )
			return false;
		for ( int i = 0; i < Size(); ++i )
			if ( m_list[i] != rhs.m_list[i] )
				return false;
		return true;
	}

	friend wostream & operator<< (wostream & out, NobPtrList<T> const & v)
	{
		out << OPEN_BRACKET << v.m_list.size() << L":";
		for ( auto & it: v.m_list )
		{
			out << it->GetId();
			if ( &it == &v.m_list.back() )
				break;
			out << SEPARATOR;
		}
		out << CLOSE_BRACKET;
		return out;
	}

	MicroMeterLine const CalcMaxDistLine() // find two nobs with maximum distance
	{
		MicroMeter     maxDist { 0.0_MicroMeter };   	
		MicroMeterLine lineMax { MicroMeterLine::ZERO_VAL() };
		for ( T * it1 : m_list )
		for ( T * it2 : m_list )    //TODO: optimize
		{
			auto const line { MicroMeterLine( it1->GetPos(), it2->GetPos() ) };
			auto const dist { line.Length() };
			if ( dist > maxDist )
			{
				maxDist = dist;
				lineMax = line;
			}
		}
		return lineMax;
	}

	void SortAccToDistFromLine(MicroMeterLine const & line)
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

	inline static wchar_t const OPEN_BRACKET  { L'(' };
	inline static wchar_t const SEPARATOR     { L',' };
	inline static wchar_t const CLOSE_BRACKET { L')' };

private:
	vector<T *> m_list;
};
