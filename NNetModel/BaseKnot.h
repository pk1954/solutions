// BaseKnot.h
//
// NNetModel
//
// geometry aspects of all kinds of knots

#pragma once

#include <vector>
#include "MoreTypes.h"
#include "Shape.h"

using std::vector;

class GraphicsInterface;
class PixelCoordsFp;
class NNetModel;

class BaseKnot : public Shape
{
public:
	BaseKnot
	( 
		MicroMeterPoint const center,
		tShapeType      const type,
		MicroMeter      const extension
 	)
	  : Shape( type ),
		m_center( center ),
		m_extension( extension )
	{
	}

	MicroMeterPoint GetPosition( )  const { return m_center;	}
	MicroMeter      GetExtension( ) const { return m_extension;	}

	bool IsPointInShape( NNetModel const &, MicroMeterPoint const & ) const;

	void AddIncomming  ( ShapeId const );
	void AddOutgoing   ( ShapeId const );
	void RemoveIncoming( ShapeId const );
	void RemoveOutgoing( ShapeId const );

	bool HasIncoming( ) const { return ! m_incoming.empty(); }
	bool HasOutgoing( ) const { return ! m_outgoing.empty(); }
	bool IsOrphan( )    const {	return m_incoming.empty() && m_outgoing.empty(); }

	void Apply2AllIncomingPipelines( std::function<void(ShapeId &)> const & func )
	{
		for ( auto & id : m_incoming )
		{
			func( id );
		}
	}

	void Apply2AllOutgoingPipelines( std::function<void(ShapeId &)> const & func )
	{
		for ( auto & id : m_outgoing )
		{
			func( id );
		}
	}

	void Apply2AllIncomingPipelinesConst( std::function<void(ShapeId const)> const & func ) const
	{
		for ( auto id : m_incoming )
		{
			func( id );
		}
	}

	void Apply2AllOutgoingPipelinesConst( std::function<void(ShapeId const)> const & func ) const
	{
		for ( auto id : m_outgoing )
		{
			func( id );
		}
	}

	void Apply2AllConnectedPipelines( std::function<void(ShapeId &)> const & func )
	{
		Apply2AllIncomingPipelines( [&]( ShapeId & idPipeline ) { func( idPipeline ); } );
		Apply2AllOutgoingPipelines( [&]( ShapeId & idPipeline ) { func( idPipeline ); } );
	}

	void Apply2AllConnectedPipelinesConst( std::function<void(ShapeId const)> const & func ) const
	{
		Apply2AllIncomingPipelinesConst( [&]( ShapeId idPipeline ) { func( idPipeline ); } );
		Apply2AllOutgoingPipelinesConst( [&]( ShapeId idPipeline ) { func( idPipeline ); } );
	}

	virtual void FixShapeIds( ShapeId const idLimit )
	{
		Shape::FixShapeIds( idLimit );
		Apply2AllConnectedPipelines( [&]( ShapeId & idPipeline ) { ::FixShapeId( idPipeline, idLimit ); } );
	}

	virtual void MoveTo( NNetModel &, MicroMeterPoint const & );

protected:

	vector<ShapeId> m_incoming;
	vector<ShapeId> m_outgoing;

	void drawPolygon( PixelCoordsFp const &, int const, COLORREF const, MicroMeterPoint const, MicroMeter const ) const;
	void drawPolygon( PixelCoordsFp const &, int const, COLORREF const, MicroMeter const ) const;

private:

	MicroMeterPoint m_center;
	MicroMeter      m_extension;
};
