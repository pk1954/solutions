// ShapeList.cpp 
//
// NNetModel

#include "stdafx.h"
#include "ModelUtilities.h"
#include "ShapeList.h"

using std::move;

void ShapeList::checkShape( Shape const & shape ) const
{
	switch ( shape.GetShapeType().GetValue() )
	{
	case ShapeType::Value::inputNeuron:
	case ShapeType::Value::neuron:
	case ShapeType::Value::knot:
		static_cast<BaseKnot const &>( shape ).CheckShape();
		break;

	case ShapeType::Value::pipe:
		static_cast<Pipe const &>( shape ).CheckShape();
		break;

	default:
		assert( false );
	}
}

void ShapeList::SetShapeErrorHandler( ShapeErrorHandler * const p ) 
{ 
	m_pShapeErrorHandler = p; 
}

UPShape ShallowCopy( Shape const & shape )
{
	switch ( shape.GetShapeType().GetValue() )
	{
	case ShapeType::Value::inputNeuron:
		return make_unique<InputNeuron>( static_cast<InputNeuron const &>( shape ) );

	case ShapeType::Value::knot:
		return make_unique<Knot>( static_cast<Knot const &>( shape ) );

	case ShapeType::Value::neuron:
		return make_unique<Neuron>( static_cast<Neuron const &>( shape ) );

	case ShapeType::Value::pipe:
		return make_unique<Pipe>( static_cast<Pipe const &>( shape ) );

	default:
		assert( false );
		return nullptr;
	}
}

bool ShapeList::operator==( ShapeList const & other ) const
{
	if ( m_list.size() != other.m_list.size() )
		return false;
	for ( int i = 0; i < m_list.size(); ++i )
	{
		Shape const * pShape      {       m_list[i].get() };
		Shape const * pShapeOther { other.m_list[i].get() };
		if ( (pShape == nullptr) != (pShapeOther == nullptr) )
		{
			return false;
		}
		else if ( (pShape != nullptr) && (pShapeOther != nullptr) )
		{
			if ( *pShape != *pShapeOther )
				return false;
		}
	}
	return true;
}

Shape * const ShapeList::RemoveShape( ShapeId const id )	
{
	assert( IsDefined( id ) );
	assert( IsValidShapeId( id ) );

	if ( GetAt( id ) )
		GetAt( id )->DecCounter();

	return m_list[id.GetValue()].release();
}

void ShapeList::SetShape2Slot( ShapeId const id, UPShape upShape )	 // only for special situations
{                                                        // read model from script
	assert( IsDefined( id ) );                           // or copy model
	assert( IsValidShapeId( id ) );
	assert( IsEmptySlot( id ) );
	assert( upShape );

	upShape->IncCounter(); //-V522
	m_list[id.GetValue()] = move(upShape);
}

Shape * const ShapeList::ReplaceShape( ShapeId const id, UPShape upT )	
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

void ShapeList::LinkShape( Shape const & shapeSrc, function< Shape * (Shape const *)> const & dstFromSrc ) const
{
	if ( Shape * pShapeDst { dstFromSrc( & shapeSrc ) } )
	{
		Shape & shapeDst { * pShapeDst };
		if ( shapeSrc.IsPipe( ) )
		{
			Pipe const & pipeSrc { static_cast<Pipe const &>( shapeSrc ) };
			Pipe       & pipeDst { static_cast<Pipe       &>( shapeDst ) };
			pipeDst.SetStartKnot( static_cast<BaseKnot *>( dstFromSrc( pipeSrc.GetStartKnotPtr() ) ) );
			pipeDst.SetEndKnot  ( static_cast<BaseKnot *>( dstFromSrc( pipeSrc.GetEndKnotPtr  () ) ) );
		}
		else
		{
			Connections const & srcConn { static_cast<BaseKnot const &>( shapeSrc ).m_connections };
			Connections       & dstConn { static_cast<BaseKnot       &>( shapeDst ).m_connections };
			dstConn.ClearOutgoing();
			dstConn.ClearIncoming();
			srcConn.Apply2AllOutPipes([&](Pipe const & pipeSrc) {dstConn.AddOutgoing( static_cast<Pipe *>(dstFromSrc( & pipeSrc )));});
			srcConn.Apply2AllInPipes ([&](Pipe const & pipeSrc) {dstConn.AddIncoming( static_cast<Pipe *>(dstFromSrc( & pipeSrc )));});
		}
	}
}

void ShapeList::Push( UPShape upShape )	
{
	if ( upShape )
	{
		upShape->SetId( IdNewSlot() );
		upShape->IncCounter();
	}
	m_list.push_back( move(upShape) );
}

void ShapeList::copy( const ShapeList & rhs )
{
	rhs.CheckShapeList();

	m_list.resize( Cast2Long(rhs.m_list.size()) );

	for ( auto const & pShapeSrc : rhs.m_list )
	{
		if ( pShapeSrc )
		{
			SetShape2Slot( pShapeSrc->GetId(), ShallowCopy( * pShapeSrc ) );
		}
	}

	for ( auto const & pShapeSrc : rhs.m_list )
	{
		if ( pShapeSrc )
			LinkShape( * pShapeSrc, [&](Shape const * pSrc){ return GetAt(pSrc->GetId()); });
	}

	m_pShapeErrorHandler = rhs.m_pShapeErrorHandler;
}

ShapeList::~ShapeList()
{
	Clear( );
}

ShapeList::ShapeList( const ShapeList & rhs ) // copy constructor
{
	copy( rhs );
}

ShapeList & ShapeList::operator= ( const ShapeList & rhs ) // copy assignment
{
	if (this != &rhs)
		copy( rhs );
	return * this;
}

void ShapeList::CheckShapeList( ) const
{
#ifdef _DEBUG
	Apply2All( [&]( Shape const & shape ) { checkShape( shape ); } );
#endif
}

void ShapeList::Dump( ) const
{
	Apply2All( [&]( Shape const & shape ) { shape.Dump( ); } );
}

MicroMeterRect const ShapeList::ComputeEnclosingRect( ) const
{
	MicroMeterRect rect { MicroMeterRect::ZERO_VAL() };
	for ( auto const & pShape : m_list )
		Expand( rect, pShape.get() );
	return rect;
}

ShapeId const ShapeList::FindShapeAt
( 
	MicroMeterPoint const   pnt, 
	ShapeCrit       const & crit 
) const
{
	for ( size_t i = m_list.size(); i --> 0; )	
	{
		Shape * pShape = m_list[i].get();
		if ( pShape && crit( * pShape ) && pShape->IsPointInShape( pnt ) ) 
			return pShape->GetId();
	};
	return NO_SHAPE;
}

bool const ShapeList::AnyShapesSelected( ) const
{
	return Apply2AllB<Shape>( [&]( Shape const & shape ) { return shape.IsSelected(); } );
}

void ShapeList::CallErrorHandler( ShapeId const id ) const
{
	if ( m_pShapeErrorHandler )
	{
		(* m_pShapeErrorHandler)( id );
	}
}

void ShapeList::Apply2All( function<void(Shape const &)> const & func ) const
{
	for ( auto const & it : m_list )
	{ 
		if ( it.get() )
			func( * it.get() ); 
	}
}                        

void ShapeList::Apply2All( function<void(Shape &)> const & func )
{
	for ( auto & it : m_list )
	{ 
		if ( it.get() )
			func( * it.get() ); 
	}
}                        

bool const ShapeList::Apply2AllB( ShapeCrit const & func ) const
{
	for ( auto & it : m_list )
	{ 
		if ( it.get() && func( * it.get() ) )
			return true;
	}
	return false;
}

void ShapeList::SelectAllShapes( tBoolOp const op ) 
{ 
	Apply2All( [&](Shape & s) { s.Select( op ); } ); 
}

void ShapeList::Append( ShapeList const &  list2Append )
{
//	XXXXXXXXX
}
