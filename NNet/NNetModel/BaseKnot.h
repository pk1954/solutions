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
class Pipe;

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
		m_extension( extension )
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

	void AddIncoming    ( Pipe * const );
	void AddOutgoing    ( Pipe * const );
	void RemoveIncoming ( Pipe * const );
	void RemoveOutgoing ( Pipe * const );
	void ReplaceIncoming( Pipe * const, Pipe * const );
	void ReplaceOutgoing( Pipe * const, Pipe * const );

	bool   HasIncoming( )                const { return ! m_incoming.empty(); }
	bool   HasOutgoing( )                const { return ! m_outgoing.empty(); }
	size_t GetNrOfIncomingConnections( ) const { return m_incoming.size(); }
	size_t GetNrOfOutgoingConnections( ) const { return m_outgoing.size(); }
	size_t GetNrOfConnections( )         const { return m_incoming.size() + m_outgoing.size(); }
	bool   IsOrphan( )                   const { return m_incoming.empty() && m_outgoing.empty(); }
	bool   IsOrphanedKnot( )             const { return (GetShapeType().IsKnotType()) && IsOrphan(); }

	void ClearIncoming( ) 
	{
		LockShape();
		m_incoming.clear(); 
		UnlockShape();
	}

	void ClearOutgoing( ) 
	{ 
		LockShape();
		m_outgoing.clear(); 
		UnlockShape();
	}

	bool IsPrecursorOf( ShapeId const );
	bool IsSuccessorOf( ShapeId const );

	void Apply2AllInPipes( function<void(Pipe * const)> const & func )
	{
		apply2All( m_incoming, func );
	}

	void Apply2AllOutPipes( function<void(Pipe * const)> const & func )
	{
		apply2All( m_outgoing, func );
	}

	bool Apply2AllInPipesB( function<bool(Pipe * const)> const & func )
	{
		return apply2AllB( m_incoming, func );
	}

	bool Apply2AllOutPipesB( function<bool(Pipe * const)> const & func )
	{
		return apply2AllB( m_outgoing, func );
	}

	void Apply2AllConnectedPipes( function<void(Pipe * const)> const & func )
	{
		Apply2AllInPipes ( [&]( Pipe * const pipe ) { func( pipe ); } );
		Apply2AllOutPipes( [&]( Pipe * const pipe ) { func( pipe ); } );
	}

	void Apply2AllConnectedPipesB( function<bool(Pipe * const)> const & func )
	{
		Apply2AllInPipesB ( [&]( Pipe * const pipe ) { return func( pipe ); } );
		Apply2AllOutPipesB( [&]( Pipe * const pipe ) { return func( pipe ); } );
	}

	virtual void MoveShape( MicroMeterPoint const & );

	virtual void DrawNeuronText( PixelCoordsFp & ) const;

protected:

	vector<Pipe *> m_incoming;
	vector<Pipe *> m_outgoing;

	void drawCircle( PixelCoordsFp const &, D2D1::ColorF const, MicroMeterPoint const, MicroMeter const ) const;
	void drawCircle( PixelCoordsFp const &, D2D1::ColorF const, MicroMeter const ) const;

	PixelRect const GetPixRect4Text( PixelCoordsFp const & ) const;
	void      const DisplayText( PixelRect const, wstring const ) const;

private:
	void apply2All ( vector<Pipe *> const &, function<void(Pipe * const)> const & );
	bool apply2AllB( vector<Pipe *> const &, function<bool(Pipe * const)> const & );

	MicroMeterPoint     m_center;
	MicroMeter          m_extension;
	IDWriteTextFormat * m_pTextFormat { nullptr };
};

BaseKnot const * Cast2BaseKnot( Shape const * );
BaseKnot       * Cast2BaseKnot( Shape       * );
