// UPShapeList.cpp 
//
// NNetModel

#include "stdafx.h"
#include "Knot.h"
#include "Pipe.h"
#include "BaseKnot.h"
#include "Neuron.h"
#include "Connector.h"
#include "ClosedConnector.h"
#include "InputNeuron.h"
#include "OutputNeuron.h"
#include "ShapeIdList.h"
#include "UPShapeList.h"

using std::move;

void UPShapeList::Clear()
{ 
	m_list.clear(); 
	m_shapesOfType.fill( 0 );
}

//void UPShapeList::checkShape( Shape const & shape ) const
//{
//	switch ( shape.GetShapeType().GetValue() )
//	{
//	case ShapeType::Value::inputNeuron:
//	case ShapeType::Value::outputNeuron:
//	case ShapeType::Value::neuron:
//	case ShapeType::Value::knot:
//		static_cast<BaseKnot const &>(shape).CheckShape();
//		break;
//
//	case ShapeType::Value::pipe:
//		static_cast<Pipe const &>(shape).CheckShape();
//		break;
//
//	case ShapeType::Value::connector:
//		static_cast<Connector const &>(shape).CheckShape();
//		break;
//
//	case ShapeType::Value::closedConnector:
//		static_cast<ClosedConnector const &>(shape).CheckShape();
//		break;
//
//	default:
//		assert( false );
//	}
//}

void UPShapeList::SetErrorHandler( ShapeErrorHandler * const p ) 
{ 
	m_pShapeErrorHandler = p; 
}

UPShape ShallowCopy( Shape const & shape )
{
	switch ( shape.GetShapeType().GetValue() )
	{
	case ShapeType::Value::connector:
		return Copy<Connector>(shape);

	case ShapeType::Value::inputNeuron:
		return Copy<InputNeuron>(shape);

	case ShapeType::Value::outputNeuron:
		return Copy<OutputNeuron>(shape);

	case ShapeType::Value::knot:
		return Copy<Knot>(shape);

	case ShapeType::Value::neuron:
		return Copy<Neuron>(shape);

	case ShapeType::Value::pipe:
		return Copy<Pipe>(shape);

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

UPShape UPShapeList::ExtractShape( ShapeId const id )	
{
	assert( IsDefined(id) );
	assert( IsValidShapeId(id) );

	decCounter(id);

	return move( m_list[id.GetValue()] );
}


void UPShapeList::SetShape2Slot( ShapeId const id, UPShape upShape ) // only for special situations
{                                                                    // read model from script
	assert( IsDefined(id) );
	assert( IsValidShapeId(id) );
	assert( IsEmptySlot(id) );
	assert( upShape );

	incCounter(upShape);
	m_list[id.GetValue()] = move(upShape);
}

void UPShapeList::SetShape2Slot( UPShape upShape )	 // only for special situations
{                 
	ShapeId const id { upShape->GetId() };
	SetShape2Slot(id, move(upShape));
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

void UPShapeList::LinkShape
(
	Shape                             const & shapeSrc, 
	function<Shape * (Shape const *)> const & dstFromSrc
) const
{
	if ( Shape * pShapeDst { dstFromSrc(& shapeSrc) } )
		pShapeDst->Link(shapeSrc, dstFromSrc);
}

ShapeId const UPShapeList::Push( UPShape upShape )	
{
	ShapeId idNewSlot { IdNewSlot() };
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
			LinkShape(*pShapeSrc, [&](Shape const *pSrc){ return GetAt(pSrc->GetId()); });
	}

	m_pShapeErrorHandler = rhs.m_pShapeErrorHandler;
}

UPShapeList::~UPShapeList()
{
	Clear();
}

UPShapeList::UPShapeList( const UPShapeList & rhs ) // copy constructor
{
	copy( rhs );
}

UPShapeList & UPShapeList::operator= ( const UPShapeList & rhs ) // copy assignment
{
	if (this != &rhs)
	{
		Clear();
		copy( rhs );
	}
	return * this;
}

void UPShapeList::CheckShapeList() const
{
#ifdef _DEBUG
	Apply2All( [&](Shape const & shape) { shape.CheckShape(); } );
#endif
}

void UPShapeList::Dump() const
{
	Apply2All( [&](Shape const & shape) { shape.Dump(); } );
}

MicroMeterRect const UPShapeList::CalcEnclosingRect( SelMode const mode ) const
{
	MicroMeterRect rect { MicroMeterRect::ZERO_VAL() };
	for ( auto const & pShape : m_list )
		if ( pShape && ((mode == SelMode::allShapes) || pShape->IsSelected()) )
			pShape->Expand( rect );
	return rect;
}

ShapeId const UPShapeList::FindShapeAt
( 
	MicroMeterPoint const   pnt, 
	ShapeCrit       const & crit 
) const
{
	for (size_t i = m_list.size(); i --> 0;)	
	{
		Shape * pShape = m_list[i].get();
		if ( pShape && crit(* pShape) && pShape->Includes(pnt) ) 
			return pShape->GetId();
	};
	return ShapeId( NO_SHAPE );
}

bool const UPShapeList::AnyShapesSelected() const
{
	return Apply2AllB<Shape>([&](Shape const & s) { return s.IsSelected(); });
}

void UPShapeList::CallErrorHandler(ShapeId const id) const
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

void UPShapeList::SelectAllShapes(bool const bOn) 
{ 
	Apply2All( [&](Shape & s) { s.Select(bOn); } ); 
}

ShapeIdList UPShapeList::Append( UPShapeList & list2Append )
{
	ShapeIdList idList;
	long offset { Size() };
	for ( auto & upShape : list2Append.m_list )
	{
		if ( upShape )
		{
			ShapeId id { upShape->GetId() + offset };
			upShape->SetId( id );
			idList.Add( id );
		}
		m_list.push_back( move(upShape) );
	}
	return idList;
}

UPShapeList UPShapeList::ExtractShapes(ShapeIdList idList)
{
	UPShapeList shapeList;
	idList.Apply2All([&](ShapeId const &id){ shapeList.Push(ExtractShape(id)); } );
	return shapeList;
}
