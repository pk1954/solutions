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
class Pipeline;

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

	virtual mV GetNextOutput( ) const = 0;

	static bool TypeFits( tShapeType const type ) { return IsBaseKnotType( type ); }

	MicroMeterPoint GetPosition( )  const { return m_center;	}
	MicroMeter      GetExtension( ) const { return m_extension;	}

	bool IsPointInShape( MicroMeterPoint const & ) const;

	virtual bool IsInRect( MicroMeterRect const & umRect ) const 
	{ 
		return umRect.Includes( m_center );
	}

	void AddIncoming    ( Pipeline * const );
	void AddOutgoing    ( Pipeline * const );
	void RemoveIncoming ( Pipeline * const );
	void RemoveOutgoing ( Pipeline * const );
	void ReplaceIncoming( Pipeline * const, Pipeline * const );
	void ReplaceOutgoing( Pipeline * const, Pipeline * const );

	bool   HasIncoming( )                const { return ! m_incoming.empty(); }
	bool   HasOutgoing( )                const { return ! m_outgoing.empty(); }
	size_t GetNrOfIncomingConnections( ) const { return m_incoming.size(); }
	size_t GetNrOfOutgoingConnections( ) const { return m_outgoing.size(); }
	size_t GetNrOfConnections( )         const { return m_incoming.size() + m_outgoing.size(); }
	bool   IsOrphan( )                   const { return m_incoming.empty() && m_outgoing.empty(); }
	bool   IsOrphanedKnot( )             const { return (GetShapeType() == tShapeType::knot) && IsOrphan(); }

	void ClearIncoming( ) { m_incoming.clear(); }
	void ClearOutgoing( ) { m_outgoing.clear(); }

	bool IsPrecursorOf( ShapeId const );
	bool IsSuccessorOf( ShapeId const );

	Pipeline * const GetAxon( ) const
	{
		assert( m_outgoing.size() == 1 );
		return m_outgoing.front();
	}

	bool Apply2AllIncomingPipelines( function<bool(Pipeline * const)> const & func )
	{
		bool bResult { false };
		for ( auto pipe : m_incoming ) 
		{ 
			if ( pipe ) 
			{
				bResult = func( pipe );
				if ( bResult )
					break;
			}
		}
		return bResult;
	}

	bool Apply2AllOutgoingPipelines( function<bool(Pipeline * const)> const & func )
	{
		bool bResult { false };
		for ( auto pipe : m_outgoing ) 
		{ 
			if ( pipe ) 
			{
				bResult = func( pipe );
				if ( bResult )
					break;
			}
		}
		return bResult;
	}

	bool Apply2AllIncomingPipelinesConst( function<bool(Pipeline const * const)> const & func ) const
	{
		bool bResult { false };
		for ( auto pipe : m_incoming ) 
		{ 
			if ( pipe ) 
			{
				bResult = func( pipe );
				if ( bResult )
					break;
			}
		}
		return bResult;
	}

	bool Apply2AllOutgoingPipelinesConst( function<bool(Pipeline const * const)> const & func ) const
	{
		bool bResult { false };
		for ( auto pipe : m_outgoing ) 
		{ 
			if ( pipe ) 
			{
				bResult = func( pipe );
				if ( bResult )
					break;
			}
		}
		return bResult;
	}

	void Apply2AllConnectedPipelines( function<bool(Pipeline const *)> const & func )
	{
		Apply2AllIncomingPipelines( [&]( Pipeline const * pipe ) { return func( pipe ); } );
		Apply2AllOutgoingPipelines( [&]( Pipeline const * pipe ) { return func( pipe ); } );
	}

	void Apply2AllConnectedPipelinesConst( function<bool(Pipeline const * const)> const & func ) const
	{
		Apply2AllIncomingPipelinesConst( [&]( Pipeline const * const pipe ) { return func( pipe ); } );
		Apply2AllOutgoingPipelinesConst( [&]( Pipeline const * const pipe ) { return func( pipe ); } );
	}

	virtual void MoveShape( MicroMeterPoint const & );

	virtual void DrawNeuronText( PixelCoordsFp & ) const;

protected:

	vector<Pipeline *> m_incoming;
	vector<Pipeline *> m_outgoing;

	void drawCircle( PixelCoordsFp const &, COLORREF const, MicroMeterPoint const, MicroMeter const ) const;
	void drawCircle( PixelCoordsFp const &, COLORREF const, MicroMeter const ) const;

	PixelRect const GetPixRect4Text( PixelCoordsFp const & ) const;
	void      const DisplayText( PixelRect const, wstring const ) const;

private:

	MicroMeterPoint     m_center;
	MicroMeter          m_extension;
	IDWriteTextFormat * m_pTextFormat;
};

BaseKnot const * Cast2BaseKnot( Shape const * );
BaseKnot       * Cast2BaseKnot( Shape       * );
