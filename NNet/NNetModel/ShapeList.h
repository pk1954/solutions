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
using std::unique_ptr;
using std::make_unique;

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

	void Dump( ) const
	{
		Apply2All( [&]( Shape const & shape ) { shape.Dump( ); } );
	}

	ShapeId const IdNewSlot( )	
	{
		return ShapeId( Cast2Long(m_list.size()) );
	}


	void Push( UPShape upShape )	
	{
		if ( upShape )
		{
			upShape->SetId( IdNewSlot() );
			upShape->IncCounter();
		}
		m_list.push_back( move(upShape) );
	}

	template <typename T>
	unique_ptr<T> Pop( )
	{
		unique_ptr<T> upT { unique_ptr<T>( static_cast<T*>(m_list.back().release()) ) };
		upT->DecCounter();
		m_list.pop_back( );
		return move( upT );
	}

	Shape * RemoveShape( ShapeId const id )	
	{
		assert( IsDefined( id ) );
		assert( IsValidShapeId( id ) );

		if ( GetAt( id ) )
			GetAt( id )->DecCounter();

		return m_list[id.GetValue()].release();
	}

	void SetShape2Slot( ShapeId const id, UPShape upShape )	 // only for special situations
	{                                                        // read model from script
		assert( IsDefined( id ) );                           // or copy model
		assert( IsValidShapeId( id ) );
		assert( IsEmptySlot( id ) );
		assert( upShape );

		upShape->IncCounter(); //-V522
		m_list[id.GetValue()] = move(upShape);
	}

	Shape * ReplaceShape( ShapeId const id, UPShape upT )	
	{
		assert( IsDefined( id ) );
		assert( IsValidShapeId( id ) );

		if ( upT )
			upT->IncCounter();
		if ( GetAt( id ) )
			GetAt( id )->DecCounter();

		UPShape tmp = move(upT);
		m_list[id.GetValue()].swap( tmp );
		return tmp.release();
	}

	void Reset( )
	{
		m_list.clear();
	}

	Shape * const GetAt( ShapeId const id ) const 
	{
		return m_list[id.GetValue()].get();
	}

	Shape & GetRef( ShapeId const id ) 
	{
		return * m_list[id.GetValue()];
	}

	Shape * const Front( ) const 
	{
		return m_list[0].get();
	}

	bool const IsValidShapeId( ShapeId const id ) const 
	{ 
		return (0 <= id.GetValue()) && (id.GetValue() < Size()); 
	}

	bool const IsEmptySlot( ShapeId const id ) const 
	{ 
		return GetAt( id ) == nullptr; 
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
		m_list.resize( lNewSize );
	}

	void Clear( )
	{
		m_list.clear( );
	}

	void Apply2All( function<void(Shape const &)> const & func ) const
	{
		for ( auto const & it : m_list )
		{ 
			if ( it.get() )
				func( * it.get() ); 
		}
	}                        

	void Apply2All( function<void(Shape &)> const & func )
	{
		for ( auto & it : m_list )
		{ 
			if ( it.get() )
				func( * it.get() ); 
		}
	}                        

	bool Apply2AllB( function<bool(Shape const &)> const & func )
	{
		for ( auto & it : m_list )
		{ 
			if ( it.get() && func( * it.get() ) )
				return true;
		}
		return false;
	}                        

	template <typename T>
	bool Apply2AllB( function<bool(T &)> const & func ) const
	{
		bool bResult { false };
		for ( auto & it : m_list )
		{
			if ( it.get() )
			{
				if ( HasType<T>( * it.get() ) )	
					bResult = func( static_cast<T &>( * it.get() ) );
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
		for ( auto & it : m_list )
		{
			if ( it.get() )
			{
				if ( HasType<T>( * it.get() ) )	
					bResult = func( static_cast<T const &>( * it.get() ) );
				if ( bResult )
					break;
			}
		}
		return bResult;
	}

	template <typename T>
	void Apply2All( function<void(T &)> const & func ) const
	{
		for ( auto & it : m_list )
		{
			if ( it.get() && ( HasType<T>( * it.get() ) ) )
				func( static_cast<T &>( * it.get ) ); 
		}
	}                        

	template <typename T>    // const version
	void Apply2All( function<void(T const &)> const & func ) const
	{
		for ( auto & it : m_list )
		{
			if ( it.get() && ( HasType<T>( * it.get() ) ) )
				func( static_cast<T const &>( * it.get ) ); 
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

	void LinkShape( Shape const &, function< Shape * (Shape const *)> const & ) const;

private:
	vector<UPShape>     m_list;
	ShapeErrorHandler * m_pShapeErrorHandler { nullptr };

	void copy ( ShapeList const & );
};

UPShape ShallowCopy( Shape const & );
