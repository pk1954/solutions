// ShapeList.h
//
// NNetModel

#pragma once

#include <vector>
#include "Shape.h"

using std::vector;

class ShapeList
{
public:
	void Add( Shape * const pShape )
	{
		m_list.push_back( pShape );
	}

	void RemoveLast( )
	{
		m_list.pop_back( );
	}

	void AddAt( Shape * const pShape, ShapeId const id )
	{
		m_list[ id.GetValue() ] = pShape;
	}

	Shape * const GetAt( ShapeId const id ) const 
	{
		return m_list[ id.GetValue() ];
	}

	void SetAt( Shape * const pShape, ShapeId const id )
	{
		m_list.at( id.GetValue() ) = pShape;
	}

	Shape * const Front( ) const 
	{
		return m_list[ 0 ];
	}

	long Size( ) const
	{
		return CastToLong( m_list.size() );
	}

	void Resize( long lNewSize )
	{
		m_list.resize( lNewSize );
	}

	void Clear( )
	{
		m_list.clear( );
	}

	void Apply2All( function<void(Shape *)> const & func )
	{
		for (Shape * pShape : m_list)    
		{ 
			func( pShape ); 
		}
	}                        

	bool Apply2AllB( function<bool(Shape *)> const & func )
	{
		for (Shape * pShape : m_list)    
		{ 
			if ( func( pShape ) )
				return true;
		}
		return false;
	}                        

	template <typename T>
	bool Apply2AllB( function<bool(T &)> const & func ) const
	{
		bool bResult { false };
		for ( auto pShape : m_list )
		{
			if ( pShape )
			{
				if ( HasType<T>( pShape ) )	
					bResult = func( static_cast<T &>( * pShape ) );
				if ( bResult )
					break;
			}
		}
		return bResult;
	}

	template <typename T>   // const version
	bool Apply2AllB( function<bool(T const &)> const & func ) const
	{
		bool bResult { false };
		for ( auto pShape : m_list )
		{
			if ( pShape )
			{
				if ( HasType<T>( pShape ) )	
					bResult = func( static_cast<T const &>( * pShape ) );
				if ( bResult )
					break;
			}
		}
		return bResult;
	}

	template <typename T>
	void Apply2All( function<void(T &)> const & func ) const
	{
		for (auto & pShape : m_list)    
		{ 
			if ( pShape )
			{
				if ( HasType<T>(pShape) ) func( static_cast<T &>( * pShape) ); 
			}
		}
	}                        

	template <typename T>    // const version
	void Apply2All( function<void(T const &)> const & func ) const
	{
		for (auto & pShape : m_list)    
		{ 
			if ( pShape )
			{
				if ( HasType<T>(pShape) ) 
					func( static_cast<T const &>( * pShape) ); 
			}
		}
	}                        


private:
	vector<Shape *> m_list;
};
