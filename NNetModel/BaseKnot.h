// BaseKnot.h
//
// NNetModel
//
// geometry aspects of all kinds of knots

#pragma once

#include <vector>
#include "PixelTypes.h"
#include "MoreTypes.h"
#include "Shape.h"

using std::vector;
using std::wstring;

class GraphicsInterface;
class PixelCoordsFp;
class NNetModel;

struct IDWriteTextFormat;

class BaseKnot : public Shape
{
public:
	BaseKnot
	( 
		NNetModel     * const pModel,
		MicroMeterPoint const center,
		tShapeType      const type,
		MicroMeter      const extension
 	)
	  : Shape( pModel, type ),
		m_center( center ),
		m_extension( extension ),
		m_pTextFormat( nullptr )
	{ }

	virtual ~BaseKnot() {}

	static bool TypeFits( tShapeType const type ) { return IsBaseKnotType( type ); }

	MicroMeterPoint GetPosition( )  const { return m_center;	}
	MicroMeter      GetExtension( ) const { return m_extension;	}

	bool IsPointInShape( MicroMeterPoint const & ) const;

	void AddIncomming   ( ShapeId const );
	void AddOutgoing    ( ShapeId const );
	void RemoveIncoming ( ShapeId const );
	void RemoveOutgoing ( ShapeId const );
	void ReplaceIncoming( ShapeId const, ShapeId const );
	void ReplaceOutgoing( ShapeId const, ShapeId const );

	bool   HasIncoming( )                const { return ! m_incoming.empty(); }
	bool   HasOutgoing( )                const { return ! m_outgoing.empty(); }
	size_t GetNrOfIncomingConnections( ) const { return m_incoming.size(); }
	size_t GetNrOfOutgoingConnections( ) const { return m_outgoing.size(); }
	size_t GetNrOfConnections( )         const { return m_incoming.size() + m_outgoing.size(); }
	bool   IsOrphan( )                   const { return m_incoming.empty() && m_outgoing.empty(); }
	bool   IsOrphanedKnot( )             const { return (GetShapeType() == tShapeType::knot) && IsOrphan(); }

	ShapeId GetPrecursor( ) const;
	ShapeId GetSuccessor( ) const;

	ShapeId const GetAxon( ) const
	{
		assert( HasAxon( ) );
		assert( m_outgoing.size() == 1 );
		return m_outgoing.front();
	}

	void Apply2AllIncomingPipelines( function<void(ShapeId &)> const & func )
	{
		for ( auto & id : m_incoming )
		{
			if ( ::IsDefined( id ) )
				func( id );
		}
	}

	void Apply2AllOutgoingPipelines( function<void(ShapeId &)> const & func )
	{
		for ( auto & id : m_outgoing )
		{
			if ( ::IsDefined( id ) )
				func( id );
		}
	}

	void Apply2AllIncomingPipelinesConst( function<void(ShapeId const)> const & func ) const
	{
		for ( auto id : m_incoming )
		{
			if ( ::IsDefined( id ) )
				func( id );
		}
	}

	void Apply2AllOutgoingPipelinesConst( function<void(ShapeId const)> const & func ) const
	{
		for ( auto id : m_outgoing )
		{
			if ( ::IsDefined( id ) )
				func( id );
		}
	}

	void Apply2AllConnectedPipelines( function<void(ShapeId &)> const & func )
	{
		Apply2AllIncomingPipelines( [&]( ShapeId & idPipeline ) { func( idPipeline ); } );
		Apply2AllOutgoingPipelines( [&]( ShapeId & idPipeline ) { func( idPipeline ); } );
	}

	void Apply2AllConnectedPipelinesConst( function<void(ShapeId const)> const & func ) const
	{
		Apply2AllIncomingPipelinesConst( [&]( ShapeId idPipeline ) { func( idPipeline ); } );
		Apply2AllOutgoingPipelinesConst( [&]( ShapeId idPipeline ) { func( idPipeline ); } );
	}

	virtual void MoveShape( MicroMeterPoint const & );

	virtual void DrawNeuronText( PixelCoordsFp & ) const;

protected:

	vector<ShapeId> m_incoming;
	vector<ShapeId> m_outgoing;

	void drawCircle( PixelCoordsFp const &, COLORREF const, MicroMeterPoint const, MicroMeter const ) const;
	void drawCircle( PixelCoordsFp const &, COLORREF const, MicroMeter const ) const;

	PixelRect const GetPixRect4Text( PixelCoordsFp const & ) const;
	void      const DisplayText( PixelRect const, wstring const ) const;

private:

	MicroMeterPoint     m_center;
	MicroMeter          m_extension;
	IDWriteTextFormat * m_pTextFormat;
};
