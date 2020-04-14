// BaseKnot.cpp
//
// NNetModel

#include "stdafx.h"
#include "assert.h"
#include <sstream> 
#include <iomanip>
#include "Geometry.h"
#include "PixelCoordsFp.h"
#include "Direct2D.h"
#include "Pipe.h"
#include "BaseKnot.h"

using std::find;
using std::begin;
using std::end;
using std::fixed;
using std::wstring;
using std::wostringstream;

void BaseKnot::apply2All
( 
	vector<Pipe *>               const & pipeList,
	function<void(Pipe * const)> const & func 
)
{
	LockShape();
	for ( auto pipe : pipeList ) 
	{ 
		if ( pipe == nullptr )
			throw std::logic_error( "nullptr in pipe list"  );
		func( pipe );
	}
	UnlockShape();
}

bool BaseKnot::apply2AllB
( 
	vector<Pipe *>               const & pipeList,
	function<bool(Pipe * const)> const & func 
)
{
	bool bResult { false };
	LockShape();
	for ( auto pipe : pipeList ) 
	{ 
		if ( pipe == nullptr )
			throw std::logic_error( "nullptr in pipe list"  );

		bResult = func( pipe );
		if ( bResult )
			break;
	}
	UnlockShape();
	return bResult;
}

void BaseKnot::Prepare( )
{
	m_mVinputBuffer = 0._mV;
	Apply2AllInPipes( [&]( auto pipe ) { m_mVinputBuffer += pipe->GetNextOutput( ); } );
}

bool BaseKnot::IsPrecursorOf( ShapeId const id )
{
	return Apply2AllOutPipesB( [&]( auto pipe ) { return pipe->GetEndKnotId( ) == id; } ); 
}

bool BaseKnot::IsSuccessorOf( ShapeId const id )
{
	return Apply2AllInPipesB( [&]( auto pipe ) { return pipe->GetStartKnotId( ) == id;	} );
}

void BaseKnot::AddIncoming( Pipe * const pPipe )
{
	assert( find( begin(m_incoming), end(m_incoming), pPipe ) == end(m_incoming) );
	m_incoming.push_back( pPipe );
}

void BaseKnot::AddOutgoing( Pipe * const pPipe )
{
	assert( find( begin(m_outgoing), end(m_outgoing), pPipe ) == end(m_outgoing) );
	m_outgoing.push_back( pPipe );
}

void BaseKnot::RemoveIncoming( Pipe * const pPipe )
{
	auto res = find( begin(m_incoming), end(m_incoming), pPipe );
	m_incoming.erase( res );
}

void BaseKnot::RemoveOutgoing( Pipe * const pPipe )
{
	auto res = find( begin(m_outgoing), end(m_outgoing), pPipe );
	m_outgoing.erase( res );
}

void BaseKnot::ReplaceIncoming( Pipe * const pPipeOld, Pipe * const pPipeNew )
{
	assert( find( begin(m_incoming), end(m_incoming), pPipeNew ) == end(m_incoming) );
	* find( begin(m_incoming), end(m_incoming), pPipeOld ) = pPipeNew;
}

void BaseKnot::ReplaceOutgoing( Pipe * const pPipeOld, Pipe * const pPipeNew )
{
	assert( find( begin(m_outgoing), end(m_outgoing), pPipeNew ) == end(m_outgoing) );
	* find( begin(m_outgoing), end(m_outgoing), pPipeOld ) = pPipeNew;
}

bool BaseKnot::IsPointInShape( MicroMeterPoint const & point ) const
{
	MicroMeterPoint const corner1 = m_center + MicroMeterPoint( + m_extension, + m_extension );
	MicroMeterPoint const corner2 = m_center + MicroMeterPoint( + m_extension, - m_extension );
	MicroMeterPoint const corner3 = m_center + MicroMeterPoint( - m_extension, + m_extension );

	return IsPointInRect< MicroMeterPoint >( point, corner1, corner2, corner3 );
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

void const BaseKnot::DisplayText( PixelRect const pixRect, wstring const text ) const
{
	static D2D1::ColorF const colF { 0.0f, 255.0f, 0.0f, 1.0f };

	m_pGraphics->DisplayText( pixRect, text, colF );
}

void BaseKnot::DrawNeuronText( PixelCoordsFp & coord ) const
{ 
	wostringstream m_wBuffer;
	m_wBuffer.precision(2);

	float fFillLevel { GetFillLevel() };
	if ( fFillLevel >= 1.0f )
		m_wBuffer << L"TRIGGER";
	else
		m_wBuffer << fixed << setw(6) << fFillLevel * 100.0f << L"%";

	PixelRect const pixRect { GetPixRect4Text( coord ) };

	DisplayText( pixRect, m_wBuffer.str( ) );
}

void BaseKnot::MoveShape( MicroMeterPoint const & delta )
{
	m_center += delta;
	for ( auto const pipe : m_incoming )
		pipe->Recalc( );

	for ( auto const pipe : m_outgoing )
		pipe->Recalc( );
}

void BaseKnot::drawCircle
(
	PixelCoordsFp   const & coord,
	D2D1::ColorF    const   colF, 
	MicroMeterPoint const   umCenter,
	MicroMeter      const   umWidth
) const
{
	m_pGraphics->DrawCircle
	( 
		coord.convert2fPixelPos( umCenter ), 
		colF, 
		coord.convert2fPixel( umWidth )
	);
}

void BaseKnot::drawCircle
(
	PixelCoordsFp const & coord,
	D2D1::ColorF  const   colF, 
	MicroMeter    const   umWidth
) const
{
	m_pGraphics->DrawCircle
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
