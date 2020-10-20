// ShapeList.h
//
// NNetModel

#pragma once

#include <vector>
#include "Shape.h"
#include "Knot.h"
#include "Pipe.h"
#include "BaseKnot.h"
#include "Neuron.h"
#include "InputNeuron.h"
#include "Connections.h"
#include "ShapeErrorHandler.h"

using std::vector;

class ShapeList
{
public:

	ShapeList() { }

	~ShapeList();

	ShapeList( const ShapeList & );
	ShapeList & operator= ( ShapeList const & );
	bool operator== ( ShapeList const & ) const;

	void checkShape( Shape const & ) const;

	void CheckShapeList( ) const
	{
#ifdef _DEBUG
		Apply2All( [&]( Shape const & shape ) { checkShape( shape ); } );
#endif
	}

	void SetShape( Shape * const, ShapeId const );	

	void Reset( )
	{
		m_list.clear();
	}

	void Add( Shape * const pShape )
	{
		m_list.push_back( pShape );
	}

	void RemoveLast( )
	{
		m_list.pop_back( );
	}

	Shape * const GetAt( ShapeId const id ) const 
	{
		return m_list[ id.GetValue() ];
	}

	Shape * const Front( ) const 
	{
		return m_list[0];
	}

	bool const IsValidShapeId( ShapeId const id ) const 
	{ 
		return (0 <= id.GetValue()) && (id.GetValue() < Size()); 
	}
	
	bool const AnyShapesSelected( ) const
	{
		return Apply2AllB<Shape>( [&]( Shape const & shape ) { return shape.IsSelected(); } );
	}

	void CallErrorHandler( ShapeId const id ) const
	{
		if ( m_pShapeErrorHandler )
		{
			(* m_pShapeErrorHandler)( id );
		}
	}

	long const Size( ) const
	{
		return Cast2Long( m_list.size() );
	}

	void Resize( long lNewSize )
	{
		m_list.resize( lNewSize, nullptr );
	}

	void Clear( )
	{
		m_list.clear( );
	}

	void Apply2All( function<void(Shape const &)> const & func ) const
	{
		for (Shape const * const pShape : m_list)
		{ 
			if ( pShape )
				func( * pShape ); 
		}
	}                        

	void Apply2All( function<void(Shape &)> const & func )
	{
		for (Shape * const pShape : m_list)    
		{ 
			if ( pShape )
				func( * pShape ); 
		}
	}                        

	bool Apply2AllB( function<bool(Shape const &)> const & func )
	{
		for (Shape * pShape : m_list)    
		{ 
			if ( pShape && func( * pShape ) )
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
				if ( HasType<T>( * pShape ) )	
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
				if ( HasType<T>( * pShape ) )	
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
				if ( HasType<T>( * pShape ) ) 
					func( static_cast<T &>( * pShape) ); 
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
				if ( HasType<T>( * pShape ) ) 
					func( static_cast<T const &>( * pShape) ); 
			}
		}
	}                        

	MicroMeterRect ComputeEnclosingRect( );

	ShapeId const FindShapeAt( MicroMeterPoint const, ShapeCrit const & ) const;

	void SetShapeErrorHandler( ShapeErrorHandler * const pHandler )
	{	
		m_pShapeErrorHandler = pHandler;
	}

	void SelectAllShapes( tBoolOp const op ) 
	{ 
		Apply2All( [&](Shape & s) { s.Select( op ); } ); 
	}

private:
	vector<Shape *>     m_list;
	ShapeErrorHandler * m_pShapeErrorHandler { nullptr };

	void    init       ( ShapeList const & );
	Shape * shallowCopy( Shape const & ) const;
//	bool    isEqual    ( Shape const &, Shape const & ) const;
};
