// ShapePtrList.h
//
// NNetModel

#pragma once

#include <vector>
#include "Shape.h"

using std::vector;

template <Shape_t T>
class ShapePtrList
{
public:

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

	friend wostream & operator<< ( wostream & out, ShapePtrList<T> const & list )
	{
		out << L"(";
		if ( ! list.IsEmpty() )
		{
			out << list.m_list[0]->GetId();
			for ( int i = 1; i < list.m_list.size(); ++i )
				out << L',' << list.m_list[i]->GetId();
		}
		out << L")";
		return out;
	}

private:
	vector<T *> m_list;
};
