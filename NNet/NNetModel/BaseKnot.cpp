// BaseKnot.cpp
//
// NNetModel

#include "stdafx.h"
#include "assert.h"
#include "Geometry.h"
#include "DrawContext.h"
#include "Pipe.h"
#include "BaseKnot.h"

using std::find;
using std::begin;
using std::end;

void BaseKnot::Prepare( )
{
	m_mVinputBuffer = 0._mV;
	for ( Pipe * pPipe : m_incoming ) 
	{ 
		if ( pPipe != nullptr )
			m_mVinputBuffer += pPipe->GetNextOutput( );
	}

	//Apply2AllInPipes_Lock( [&]( auto pPipe ) { m_mVinputBuffer += pPipe->GetNextOutput( ); } ); // slow
}

bool BaseKnot::IsPrecursorOf( ShapeId const id )
{
	return Apply2AllOutPipesB( [&]( auto pipe ) { return pipe->GetEndKnotId( ) == id; } ); 
}

bool BaseKnot::IsSuccessorOf( ShapeId const id )
{
	return Apply2AllInPipesB( [&]( auto pipe ) { return pipe->GetStartKnotId( ) == id; } );
}

void BaseKnot::clearPipeList( PipeList & list ) 
{
	LockShapeExclusive();
	list.clear(); 
	UnlockShapeExclusive();
}

void BaseKnot::addPipe( PipeList & list, Pipe * const pPipe )
{
	if ( pPipe != nullptr )
	{
		assert( find( begin(list), end(list), pPipe ) == end(list) );
		list.push_back( pPipe );
	}
}

void BaseKnot::removePipe( PipeList & list, Pipe * const pPipe )
{
	auto res = find( begin(list), end(list), pPipe );
	list.erase( res );
}

bool BaseKnot::IsPointInShape( MicroMeterPoint const & point ) const
{
	return Distance( point, m_center ) <= m_extension;
}

MicroMeterRect const BaseKnot::GetRect4Text( ) const
{
	return MicroMeterRect
	{
		GetPosition().GetX() - GetExtension(),      // left
		GetPosition().GetY() - GetExtension() / 2,  // top
		GetPosition().GetX() + GetExtension(),      // right
		GetPosition().GetY() + GetExtension()       // bottom
	};
}

void BaseKnot::MoveShape( MicroMeterPoint const & delta )
{
	m_center += delta;
}

void BaseKnot::apply2AllPipesInList( PipeList const & pipeList, PipeFunc const & func )
{
	for ( auto pPipe : pipeList ) 
	{ 
		if ( pPipe != nullptr )
		{
			pPipe->LockShapeExclusive();
			func( pPipe );
			pPipe->UnlockShapeExclusive();
		}
	}
}

void BaseKnot::apply2AllPipesInList_NoLock( PipeList const & pipeList, PipeFunc const & func )
{
	for ( auto pPipe : pipeList ) 
	{ 
		if ( pPipe != nullptr )
		{
			func( pPipe );
		}
	}
}

bool BaseKnot::apply2AllPipesInListB( PipeList const & pipeList, PipeFuncB const & func )
{
	bool bResult { false };
	for ( auto pPipe : pipeList ) 
	{ 
		if ( pPipe != nullptr )
		{
			pPipe->LockShapeExclusive();
			bResult = func( pPipe );
			pPipe->UnlockShapeExclusive();
			if ( bResult )
				break;
		}
	}
	return bResult;
}

bool BaseKnot::apply2AllPipesInListB_NoLock( PipeList const & pipeList, PipeFuncB const & func )
{
	bool bResult { false };
	for ( auto pipe : pipeList ) 
	{ 
		if ( pipe != nullptr )
		{
			bResult = func( pipe );
			if ( bResult )
				break;
		}
	}
	return bResult;
}

void BaseKnot::drawCircle
(
	DrawContext     const & context, 
	D2D1::ColorF    const   colF, 
	MicroMeterPoint const   umCenter,
	MicroMeter      const   umWidth
) const
{
	context.DrawCircle( umCenter, umWidth, colF );
}

void BaseKnot::drawCircle
(
	DrawContext  const & context, 
	D2D1::ColorF const   colF, 
	MicroMeter   const   umWidth
) const
{
	context.DrawCircle( GetPosition(), umWidth,	colF );
}

BaseKnot const * Cast2BaseKnot( Shape const * shape )
{
	assert( ! shape->IsPipe() );
	assert( ! shape->IsUndefined() );
	return static_cast<BaseKnot const *>(shape);
}

BaseKnot * Cast2BaseKnot( Shape * shape )
{
	assert( ! shape->IsPipe() );
	assert( ! shape->IsUndefined() );
	return static_cast<BaseKnot *>(shape);
}
