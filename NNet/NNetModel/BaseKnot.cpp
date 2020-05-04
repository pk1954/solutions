// BaseKnot.cpp
//
// NNetModel

#include "stdafx.h"
#include "assert.h"
#include "Geometry.h"
#include "PixelCoordsFp.h"
#include "Direct2D.h"
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

	//Apply2AllInPipes( [&]( auto pPipe ) { m_mVinputBuffer += pPipe->GetNextOutput( ); } ); // slow
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

PixelRect const BaseKnot::GetPixRect4Text( PixelCoordsFp const & coord ) const
{
	fPixelPoint const fPos   { coord.convert2fPixelPos( GetPosition() ) }; 
	fPIXEL      const fExt   { coord.convert2fPixel( GetExtension() ) };
	PixelPoint  const pixPos { convert2PixelPoint( fPos ) };
	PIXEL       const pixExt { PIXEL(static_cast<long>(fExt.GetValue())) };
	PixelRect   const pixRect
	{
		pixPos.GetX() - pixExt,      // left
		pixPos.GetY() - pixExt / 2,  // top
		pixPos.GetX() + pixExt,      // right
		pixPos.GetY() + pixExt       // bottom
	};
	return pixRect;
}

void BaseKnot::MoveShape( MicroMeterPoint const & delta )
{
	m_center += delta;
	Apply2AllConnectedPipes( [&](auto pipe) { pipe->Recalc(); } );
}

void BaseKnot::drawCircle
(
	D2D_driver      const & graphics, 
	PixelCoordsFp   const & coord,
	D2D1::ColorF    const   colF, 
	MicroMeterPoint const   umCenter,
	MicroMeter      const   umWidth
) const
{
	graphics.DrawCircle
	( 
		coord.convert2fPixelPos( umCenter ), 
		colF, 
		coord.convert2fPixel( umWidth )
	);
}

void BaseKnot::drawCircle
(
	D2D_driver    const & graphics, 
	PixelCoordsFp const & coord,
	D2D1::ColorF  const   colF, 
	MicroMeter    const   umWidth
) const
{
	graphics.DrawCircle
	( 
		coord.convert2fPixelPos( GetPosition() ), 
		colF, 
		coord.convert2fPixel( umWidth )
	);
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
