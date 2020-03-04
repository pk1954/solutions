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
		MicroMeterPoint const center,
		ShapeType       const type,
		MicroMeter      const extension
 	)
	  : Shape( type ),
		m_center( center ),
		m_extension( extension ),
		m_pTextFormat( nullptr )
	{ }

	virtual ~BaseKnot() {}

	virtual void Prepare      ( );
	virtual mV   GetNextOutput( ) const = 0;

	static bool TypeFits( ShapeType const type ) { return type.IsBaseKnotType( ); }

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
	bool   IsOrphanedKnot( )             const { return (GetShapeType().IsKnotType()) && IsOrphan(); }

	void ClearIncoming( ) 
	{
		EnterCritSect();
		m_incoming.clear(); 
		LeaveCritSect();
	}

	void ClearOutgoing( ) 
	{ 
		EnterCritSect();
		m_outgoing.clear(); 
		LeaveCritSect();
	}

	bool IsPrecursorOf( ShapeId const );
	bool IsSuccessorOf( ShapeId const );

	bool Apply2AllIncomingPipelines( function<bool(Pipeline * const)> const & func )
	{
		return apply2All( m_incoming, func );
	}

	bool Apply2AllOutgoingPipelines( function<bool(Pipeline * const)> const & func )
	{
		return apply2All( m_outgoing, func );
	}

	void Apply2AllConnectedPipelines( function<bool(Pipeline const *)> const & func )
	{
		Apply2AllIncomingPipelines( [&]( Pipeline const * pipe ) { return func( pipe ); } );
		Apply2AllOutgoingPipelines( [&]( Pipeline const * pipe ) { return func( pipe ); } );
	}

	virtual void MoveShape( MicroMeterPoint const & );

	virtual void DrawNeuronText( PixelCoordsFp & ) const;

protected:

	vector<Pipeline *> m_incoming;
	vector<Pipeline *> m_outgoing;

	void drawCircle( PixelCoordsFp const &, D2D1::ColorF const, MicroMeterPoint const, MicroMeter const ) const;
	void drawCircle( PixelCoordsFp const &, D2D1::ColorF const, MicroMeter const ) const;

	PixelRect const GetPixRect4Text( PixelCoordsFp const & ) const;
	void      const DisplayText( PixelRect const, wstring const ) const;

private:
	bool apply2All(	vector<Pipeline *> const &, function<bool(Pipeline * const)> const & );

	MicroMeterPoint     m_center;
	MicroMeter          m_extension;
	IDWriteTextFormat * m_pTextFormat;
};

BaseKnot const * Cast2BaseKnot( Shape const * );
BaseKnot       * Cast2BaseKnot( Shape       * );
