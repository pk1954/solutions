// ShapePtrList.h
//
// NNetModel

#pragma once

#include <vector>
#include "MoreTypes.h"
#include "Shape.h"

using std::vector;
using std::sort;

template <Shape_t T>
class ShapePtrList
{
public:

	ShapePtrList() {}
	virtual ~ShapePtrList() {}

	size_t const Size()    const { return m_list.size(); }
	bool   const IsEmpty() const { return m_list.empty(); }

	T       & GetFirst()       { return * m_list.at( 0 ); }
	T const & GetFirst() const { return * m_list.at( 0 ); }

	T       & GetLast()       { return * m_list.back(); }
	T const & GetLast() const { return * m_list.back(); }

	void Check() const { for (auto & it : m_list) { it->CheckShape(); }; }

	void Clear()      {	m_list.clear(); }
	void RemoveLast() {	m_list.pop_back(); }
	void Replace(T * const pDel, T * const pAdd) 
	{ 
		replace( begin(m_list), end(m_list), pDel, pAdd ); 
	}

	void Add( T * const pShape )
	{
		if ( pShape != nullptr )
		{
			assert( find( begin(m_list), end(m_list), pShape ) == end(m_list) );
			m_list.push_back( pShape );
		}
	}

	void Remove( T * const pShape )
	{
		auto res = find( begin(m_list), end(m_list), pShape );
		assert( res != end(m_list) );
		m_list.erase( res );
	}

	void Apply2All( function<void(T &)> const & func ) const
	{
		for ( auto pShape : m_list ) 
		{ 
			if ( pShape != nullptr )
				func( * pShape );
		}
	}

	bool Apply2AllB( function<bool(T const &)> const & func ) const 
	{
		bool bResult { false };
		for ( auto pShape : m_list ) 
		{ 
			if ( pShape != nullptr )
			{
				if ( func( * pShape ) )
					return true;
			}
		}
		return false;
	}

	bool operator==(ShapePtrList const & rhs) const
	{
		if ( Size() != rhs.Size() )
			return false;
		for ( int i = 0; i < Size(); ++i )
			if ( m_list[i] != rhs.m_list[i] )
				return false;
		return true;
	}

	friend wostream & operator<< (wostream & out, ShapePtrList<T> const & v)
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

	MicroMeterLine const CalcMaxDistLine() // find two shapes with maximum distance
	{
		MicroMeter     maxDist { 0.0_MicroMeter };   	
		MicroMeterLine lineMax { MicroMeterLine::ZERO_VAL() };
		for ( T * it1 : m_list )
		for ( T * it2 : m_list )
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
