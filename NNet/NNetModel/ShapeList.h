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

	ShapeList(  )
	{
	}

	~ShapeList( )
	{
		Reset( );
	}

	ShapeList & operator= ( const ShapeList & src )
	{
		m_list.resize( CastToLong(src.m_list.size()) );

		for ( Shape * const pShape : src.m_list )
		{
			if ( pShape )
				AddAt( ShallowCopy( pShape ), pShape->GetId() ); 
		}

		for ( Shape * const pShape : src.m_list )
		{
			if ( pShape )
			{
				ShapeId const   id       { pShape->GetId() };
				Shape   const & shapeSrc { * pShape };
				Shape         & shapeDst { * m_list.at(id.GetValue()) };
				if ( shapeSrc.IsPipe( ) )
				{
					Pipe const & pipeSrc { static_cast<Pipe const &>( shapeSrc ) };
					Pipe       & pipeDst { static_cast<Pipe       &>( shapeDst ) };
					pipeDst.SetStartKnot( static_cast<BaseKnot *>( m_list.at( pipeSrc.GetStartKnotId().GetValue() )) );
					pipeDst.SetEndKnot  ( static_cast<BaseKnot *>( m_list.at( pipeSrc.GetEndKnotId  ().GetValue() )) );
				}
				else
				{
					Connections const & srcConn { static_cast<BaseKnot const &>( shapeSrc ).m_connections };
					Connections       & dstConn { static_cast<BaseKnot       &>( shapeDst ).m_connections };
					dstConn.ClearOutgoing();
					dstConn.ClearIncoming();
					auto dstFromSrc = [&](Pipe const & pipeSrc){ return static_cast<Pipe *>(m_list.at(pipeSrc.GetId().GetValue())); };
					srcConn.Apply2AllOutPipes( [&]( Pipe const & pipeSrc ) { dstConn.AddOutgoing( dstFromSrc( pipeSrc ) ); } );
					srcConn.Apply2AllInPipes ( [&]( Pipe const & pipeSrc ) { dstConn.AddIncoming( dstFromSrc( pipeSrc ) ); } );
				}
			}
		}
		m_pShapeErrorHandler = src.m_pShapeErrorHandler;
		return * this;
	}

	bool IsEqual( ShapeList const & other ) const
	{
		size_t iMax { max( m_list.size(), other.m_list.size() ) };
		for ( int i = 0; i < iMax; ++i )
		{
			Shape const * pShape      { (i >=       m_list.size()) ? nullptr :       m_list[i] };
			Shape const * pShapeOther { (i >= other.m_list.size()) ? nullptr : other.m_list[i] };
			if ( (pShape == nullptr) != (pShapeOther == nullptr) )
			{
				return false;
			}
			else if ( (pShape != nullptr) && (pShapeOther != nullptr) )
			{
				if ( ! isEqual( * pShape, * pShapeOther ) )
					return false;
			}
		}
		return true;
	}

	void Reset( )
	{
		for (auto pShape : m_list)
			delete pShape;
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

	void AddAt( Shape * const pShape, ShapeId const id )
	{
		m_list[ id.GetValue() ] = pShape;
	}

	Shape * const GetAt( ShapeId const id ) const 
	{
		return m_list[ id.GetValue() ];
	}

	Shape * const Front( ) const 
	{
		return m_list[0];
	}

	bool const IsValidShapeId  ( ShapeId const id ) const { return id.GetValue() <  Size(); }
	bool const IsInvalidShapeId( ShapeId const id ) const { return id.GetValue() >= Size(); }

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

	void SetShape( Shape * const pShape, ShapeId const id )	
	{
		if ( IsUndefined( id ) || IsInvalidShapeId( id ) )
		{
			CallErrorHandler( id );  
			return;
		}

		if ( pShape )
			pShape->IncCounter();

		Shape ** const ppShapeOld { & m_list[ id.GetValue() ] };

		if ( * ppShapeOld )
			(* ppShapeOld)->DecCounter();

		* ppShapeOld = pShape; 
	}

	long const Size( ) const
	{
		return CastToLong( m_list.size() );
	}

	void Resize( long lNewSize )
	{
		m_list.resize( lNewSize, nullptr );
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

	void Apply2AllShapes( function<void(Shape &)> const & func ) const
	{
		for (auto & pShape : m_list)    
		{ 
			if ( pShape )
			{
				func( static_cast<Shape &>( * pShape) ); 
			}
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

	MicroMeterRect ComputeEnclosingRect( )
	{
		MicroMeterRect rect { MicroMeterRect::ZERO_VAL() };
		for ( const auto & pShape : m_list )
		{
			if ( pShape )
			{
				if ( pShape->IsBaseKnot() )
				{
					rect.Expand( Cast2BaseKnot( pShape )->GetPosition() );
				}
				else if ( pShape->IsPipe() )
				{
					rect.Expand( Cast2Pipe( pShape )->GetStartPoint() );
					rect.Expand( Cast2Pipe( pShape )->GetEndPoint() );
				}
				else
				{
					assert( false );
				}
			}
		}
		return rect;
	}

	ShapeId const FindShapeAt
	( 
		MicroMeterPoint const   pnt, 
		ShapeCrit       const & crit 
	) const
	{
		for ( size_t i = m_list.size(); i --> 0; )	
		{
			Shape * pShape = m_list[i];
			if ( pShape && crit( * pShape ) && pShape->IsPointInShape( pnt ) ) 
				return pShape->GetId();
		};
		return NO_SHAPE;
	}

	void SetShapeErrorHandler( ShapeErrorHandler * const pHandler )
	{	
		m_pShapeErrorHandler = pHandler;
	}

	void SelectAllShapes( tBoolOp const op ) 
	{ 
		Apply2AllShapes( [&](Shape  &s) { s.Select( op ); } ); 
	}

private:
	vector<Shape *>     m_list;
	ShapeErrorHandler * m_pShapeErrorHandler { nullptr };

	Shape * ShallowCopy( Shape const * const pShape ) const
	{
		if ( pShape )
		{
			switch ( pShape->GetShapeType().GetValue() )
			{
			case ShapeType::Value::inputNeuron:
				return new InputNeuron( static_cast<InputNeuron const &>( * pShape ) );

			case ShapeType::Value::knot:
				return new Knot( static_cast<Knot const &>( * pShape ) );

			case ShapeType::Value::neuron:
				return new Neuron( static_cast<Neuron const &>( * pShape ) );

			case ShapeType::Value::pipe:
				return new Pipe( static_cast<Pipe const &>( * pShape ) );

			default:
				assert( false );
			}
		}
		return nullptr;
	}

	bool isEqual( Shape const & shapeA, Shape const & shapeB ) const
	{
		if ( shapeA.GetShapeType().GetValue() != shapeB.GetShapeType().GetValue() )
			return false;

		switch ( shapeA.GetShapeType().GetValue() )
		{
		case ShapeType::Value::inputNeuron:
			if (! IS_EQUAL<InputNeuron>( shapeA, shapeB ) ) 
				return false;
			break;

		case ShapeType::Value::knot:
			if (! IS_EQUAL<Knot>( shapeA, shapeB ) ) 
				return false;
			break;

		case ShapeType::Value::neuron:
			if (! IS_EQUAL<Neuron>( shapeA, shapeB ) ) 
				return false;
			break;

		case ShapeType::Value::pipe:
			if (! IS_EQUAL<Pipe>( shapeA, shapeB ) ) 
				return false;
			break;

		default:
			assert( false );
			return false;
		}
		return true;
	}

};
