// BaseKnot.h
//
// NNetModel

#pragma once

#include <vector>
#include "PixelTypes.h"
#include "MoreTypes.h"
#include "Shape.h"

using std::vector;
using std::wstring;

class DrawContext;
class NNetModel;
class Pipe;

struct IDWriteTextFormat;

class BaseKnot : public Shape
{
public:

	using PipeFunc  = function<void(Pipe &)>;
	using PipeFuncB = function<bool(Pipe &)>;

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

	virtual bool IsEqual( BaseKnot const & other ) const
	{
		if ( ! Shape::IsEqual( other ) )
			return false;
		if ( m_center != other.m_center )
			return false;
		if ( m_extension != other.m_extension )
			return false;
		return true;
	}

	virtual void Prepare      ( );
 	virtual mV   GetNextOutput( ) const = 0;
	virtual void MoveShape    ( MicroMeterPoint const & );
	virtual void SetPosition  ( MicroMeterPoint const & );
	virtual bool IsInRect     ( MicroMeterRect  const & umRect ) const 
	{ 
		return umRect.Includes( m_center ); 
	}

	static bool TypeFits( ShapeType const type ) { return type.IsBaseKnotType( ); }

	MicroMeterPoint GetPosition ( ) const { return m_center; }
	MicroMeter      GetExtension( ) const { return m_extension;	}
	mV              GetVoltage  ( ) const { return m_mVinputBuffer; }

	bool IsPointInShape( MicroMeterPoint const & ) const;

	void AddIncoming( Pipe * const p ) { addPipe( m_incoming, p ); }
	void AddOutgoing( Pipe * const p ) { addPipe( m_outgoing, p ); }

	void RemoveIncoming( Pipe * const p ) { removePipe( m_incoming, p ); }
	void RemoveOutgoing( Pipe * const p ) { removePipe( m_outgoing, p ); }

	bool   HasIncoming( )                const { return ! m_incoming.empty(); }
	bool   HasOutgoing( )                const { return ! m_outgoing.empty(); }
	size_t GetNrOfIncomingConnections( ) const { return m_incoming.size(); }
	size_t GetNrOfOutgoingConnections( ) const { return m_outgoing.size(); }
	size_t GetNrOfConnections( )         const { return m_incoming.size() + m_outgoing.size(); }
	bool   IsOrphan( )                   const { return m_incoming.empty() && m_outgoing.empty(); }
	bool   IsOrphanedKnot( )             const { return IsKnot() && IsOrphan(); }

	void ClearIncoming( ) { clearPipeList( m_incoming ); }
	void ClearOutgoing( ) { clearPipeList( m_outgoing ); }

	bool IsPrecursorOf( ShapeId const );
	bool IsSuccessorOf( ShapeId const );

	void Apply2AllInPipes ( PipeFunc const & func ) const { apply2AllPipesInList( m_incoming, func ); }
	void Apply2AllOutPipes( PipeFunc const & func ) const { apply2AllPipesInList( m_outgoing, func ); }

	bool Apply2AllInPipesB ( PipeFuncB const & func ) const { return apply2AllPipesInListB( m_incoming, func ); }
	bool Apply2AllOutPipesB( PipeFuncB const & func ) const { return apply2AllPipesInListB( m_outgoing, func ); }

	void Apply2AllConnectedPipes( PipeFunc const & func )
	{
		Apply2AllInPipes ( [&]( Pipe & pipe ) { func( pipe ); } );
		Apply2AllOutPipes( [&]( Pipe & pipe ) { func( pipe ); } );
	}

	void Apply2AllConnectedPipesB( PipeFuncB const & func )
	{
		Apply2AllInPipesB ( [&]( Pipe & pipe ) { return func( pipe ); } );
		Apply2AllOutPipesB( [&]( Pipe & pipe ) { return func( pipe ); } );
	}

protected:

	using PipeList = vector<Pipe *>;

	PipeList m_incoming;
	PipeList m_outgoing;

	void drawCircle( DrawContext const &, D2D1::ColorF const, MicroMeterPoint const, MicroMeter const ) const;
	void drawCircle( DrawContext const &, D2D1::ColorF const, MicroMeter const ) const;

	MicroMeterRect const GetRect4Text   ( ) const;

private:

	void addPipe      ( PipeList &, Pipe * const );
	void removePipe   ( PipeList &, Pipe * const );
	void clearPipeList( PipeList & );

	void apply2AllPipesInList ( PipeList const &, PipeFunc  const & ) const;
	bool apply2AllPipesInListB( PipeList const &, PipeFuncB const & ) const;

	MicroMeterPoint m_center;
	MicroMeter      m_extension;
};

BaseKnot const * Cast2BaseKnot( Shape const * );
BaseKnot       * Cast2BaseKnot( Shape       * );
