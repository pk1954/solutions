// UPShapeList.cpp 
//
// NNetModel

#include "stdafx.h"
#include "ModelUtilities.h"
#include "Knot.h"
#include "Pipe.h"
#include "BaseKnot.h"
#include "Neuron.h"
#include "InputNeuron.h"
#include "OutputNeuron.h"
#include "UPShapeList.h"

using std::move;

void UPShapeList::Clear()
{ 
	m_list.clear(); 
	m_shapesOfType.fill( 0 );
}

void UPShapeList::checkShape( Shape const & shape ) const
{
	switch ( shape.GetShapeType().GetValue() )
	{
	case ShapeType::Value::inputNeuron:
	case ShapeType::Value::outputNeuron:
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

void UPShapeList::SetErrorHandler( ShapeErrorHandler * const p ) 
{ 
	m_pShapeErrorHandler = p; 
}

UPShape ShallowCopy( Shape const & shape )
{
	switch ( shape.GetShapeType().GetValue() )
	{
	case ShapeType::Value::inputNeuron:
		return make_unique<InputNeuron>( static_cast<InputNeuron const &>( shape ) );

	case ShapeType::Value::outputNeuron:
		return make_unique<OutputNeuron>( static_cast<OutputNeuron const &>( shape ) );

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

bool UPShapeList::operator==( UPShapeList const & other ) const
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

//UPShapeList const UPShapeList::operator*= (float const f) 
//{ 
//	Apply2All<BaseKnot>([&](BaseKnot & pB) { pB *= f; });
//	return * this; 
//}
//
//UPShapeList const UPShapeList::operator+= (UPShapeList const &rhs) 
//{ 
//	if ( Size() == rhs.Size() )
//	{
//		for ( int i = 0; i < Size( ); ++i )
//		{
//			Shape * pShape    {     m_list[i].get() };
//			Shape * pShapeRhs { rhs.m_list[i].get() };
//			if ( 
//				 (pShape->GetShapeType() == pShapeRhs->GetShapeType()) &&
//				 pShape->IsBaseKnot()
//			   )
//			{
//				BaseKnot * pBaseKnot    { static_cast<BaseKnot *>(pShape)    };
//				BaseKnot * pBaseKnotRhs { static_cast<BaseKnot *>(pShapeRhs) };
//				*pBaseKnot += *pBaseKnotRhs;
//			}
//		}
//	}
//	return * this; 
//}

UPShape UPShapeList::ExtractShape( ShapeId const id )	
{
	assert( IsDefined(id) );
	assert( IsValidShapeId( id ) );

	decCounter(id);

	return move( m_list[id.GetValue()] );
}

void UPShapeList::SetShape2Slot( ShapeId const id, UPShape upShape )	 // only for special situations
{                                                                    // read model from script
	assert( IsDefined(id) );
	assert( IsValidShapeId( id ) );
	assert( IsEmptySlot( id ) );
	assert( upShape );

	incCounter(upShape);
	m_list[id.GetValue()] = move(upShape);
}

Shape * const UPShapeList::ReplaceShape( ShapeId const id, UPShape upT )	
{
	assert( IsDefined(id) );
	assert( IsValidShapeId( id ) );

	incCounter(upT);
	decCounter(id);

	UPShape tmp = move(upT);
	m_list[id.GetValue()].swap( tmp );
	return tmp.release();
}

void UPShapeList::LinkShape( Shape const & shapeSrc, function< Shape * (Shape const *)> const & dstFromSrc ) const
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

ShapeId const UPShapeList::Push( UPShape upShape )	
{
	ShapeId idNewSlot { IdNewSlot( ) };
	if ( upShape )
	{
		upShape->SetId( IdNewSlot() );
		incCounter(upShape);
	}
	m_list.push_back( move(upShape) );
	return idNewSlot;
}

void UPShapeList::copy( const UPShapeList & rhs )
{
	rhs.CheckShapeList();

	m_list.resize( Cast2Long(rhs.m_list.size()) );

	for ( auto const & pShapeSrc : rhs.m_list )
	{
		if ( pShapeSrc )
			SetShape2Slot( pShapeSrc->GetId(), ShallowCopy( * pShapeSrc ) );
	}

	for ( auto const & pShapeSrc : rhs.m_list )
	{
		if ( pShapeSrc )
			LinkShape( * pShapeSrc, [&](Shape const * pSrc){ return GetAt(pSrc->GetId()); });
	}

	m_pShapeErrorHandler = rhs.m_pShapeErrorHandler;
}

UPShapeList::~UPShapeList()
{
	Clear( );
}

UPShapeList::UPShapeList( const UPShapeList & rhs ) // copy constructor
{
	copy( rhs );
}

UPShapeList & UPShapeList::operator= ( const UPShapeList & rhs ) // copy assignment
{
	if (this != &rhs)
	{
		Clear( );
		copy( rhs );
	}
	return * this;
}

void UPShapeList::CheckShapeList( ) const
{
#ifdef _DEBUG
	Apply2All( [&]( Shape const & shape ) { checkShape( shape ); } );
#endif
}

void UPShapeList::Dump( ) const
{
	Apply2All( [&]( Shape const & shape ) { shape.Dump( ); } );
}

MicroMeterRect const UPShapeList::CalcEnclosingRect( SelMode const mode ) const
{
	MicroMeterRect rect { MicroMeterRect::ZERO_VAL() };
	for ( auto const & pShape : m_list )
		if ( (mode == SelMode::allShapes) || pShape->IsSelected( ) )
			Expand( rect, pShape.get() );
	return rect;
}

ShapeId const UPShapeList::FindShapeAt
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
	return ShapeId( NO_SHAPE );
}

bool const UPShapeList::AnyShapesSelected( ) const
{
	return Apply2AllB<Shape>( [&]( Shape const & shape ) { return shape.IsSelected(); } );
}

void UPShapeList::CallErrorHandler( ShapeId const id ) const
{
	if ( m_pShapeErrorHandler )
	{
		(* m_pShapeErrorHandler)( id );
	}
}

void UPShapeList::Apply2All( ShapeFuncC const & func ) const
{
	for ( auto const & it : m_list )
	{ 
		if ( it.get() )
			func( * it.get() ); 
	}
}                        

void UPShapeList::Apply2All( ShapeFunc const & func )
{
	for ( auto & it : m_list )
	{ 
		if ( it.get() )
			func( * it.get() ); 
	}
}                        

bool const UPShapeList::Apply2AllB( ShapeCrit const & func ) const
{
	for ( auto & it : m_list )
	{ 
		if ( it.get() && func( * it.get() ) )
			return true;
	}
	return false;
}

void UPShapeList::Apply2AllSelected( ShapeType const type, ShapeFunc const & func )
{
	Apply2All( { [&](Shape & s) { if (s.IsSelected() && (s.GetShapeType() == type)) func(s); } } );
}

void UPShapeList::Apply2AllSelected( ShapeType const type, ShapeFuncC const & func ) const
{
	Apply2All( { [&](Shape const & s) { if (s.IsSelected() && (s.GetShapeType() == type)) func(s); } } );
}

void UPShapeList::SelectAllShapes( tBoolOp const op ) 
{ 
	Apply2All( [&](Shape & s) { s.Select( op ); } ); 
}

vector<ShapeId> UPShapeList::Append( UPShapeList & list2Append )
{
	vector<ShapeId> idList;
	long offset { Size() };
	for ( auto & upShape : list2Append.m_list )
	{
		if ( upShape )
		{
			ShapeId id { upShape->GetId() + offset };
			upShape->SetId( id );
			idList.push_back( id );
		}
		m_list.push_back( move(upShape) );
	}
	return idList;
}

UPShapeList UPShapeList::ExtractShapes( vector<ShapeId> idList )
{
	UPShapeList shapeList;
	for ( auto & id : idList )
	{
		shapeList.Push( ExtractShape( id ) );
	}
	return shapeList;
}

ShapeType const UPShapeList::DetermineShapeType() const
{
	unsigned int uiNrOfInputNeurons  { CountInSelection( ShapeType::Value::inputNeuron  ) };
	unsigned int uiNrOfOutputNeurons { CountInSelection( ShapeType::Value::outputNeuron ) };
	return ((uiNrOfInputNeurons == 0) && (uiNrOfOutputNeurons == 0))
		? ShapeType::Value::undefined
		: (uiNrOfInputNeurons > uiNrOfOutputNeurons) 
		? ShapeType::Value::inputNeuron 
		: ShapeType::Value::outputNeuron;
}
