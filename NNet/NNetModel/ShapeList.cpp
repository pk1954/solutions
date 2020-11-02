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
	size_t iMax { max( m_list.size(), other.m_list.size() ) };
	for ( int i = 0; i < iMax; ++i )
	{
		Shape const * pShape      { (i >=       m_list.size()) ? nullptr :       m_list[i].get() };
		Shape const * pShapeOther { (i >= other.m_list.size()) ? nullptr : other.m_list[i].get() };
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

void ShapeList::copy( const ShapeList & rhs )
{
	m_list.resize( Cast2Long(rhs.m_list.size()) );

	rhs.CheckShapeList();

	for ( auto const & pShapeSrc : rhs.m_list )
	{
		if ( pShapeSrc )
			SetShape( ShallowCopy( * pShapeSrc ), pShapeSrc->GetId() );
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
	Reset( );
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

MicroMeterRect ShapeList::ComputeEnclosingRect( )
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
