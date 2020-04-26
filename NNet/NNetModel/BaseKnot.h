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

	using PipeFunc  = function<void(Pipe * const)>;
	using PipeFuncB = function<bool(Pipe * const)>;

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
	mV              GetVoltage( )   const { return m_mVinputBuffer; }

	bool IsPointInShape( MicroMeterPoint const & ) const;

	virtual bool IsInRect( MicroMeterRect const & umRect ) const { return umRect.Includes( m_center ); }

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

	void Apply2AllInPipes ( PipeFunc const & func ) { apply2All( m_incoming, func ); }
	void Apply2AllOutPipes( PipeFunc const & func )	{ apply2All( m_outgoing, func ); }

	bool Apply2AllInPipesB ( PipeFuncB const & func ) { return apply2AllB( m_incoming, func ); }
	bool Apply2AllOutPipesB( PipeFuncB const & func ) { return apply2AllB( m_outgoing, func ); }

	void Apply2AllConnectedPipes( PipeFunc const & func )
	{
		Apply2AllInPipes ( [&]( Pipe * const pipe ) { func( pipe ); } );
		Apply2AllOutPipes( [&]( Pipe * const pipe ) { func( pipe ); } );
	}

	void Apply2AllConnectedPipesB( PipeFuncB const & func )
	{
		Apply2AllInPipesB ( [&]( Pipe * const pipe ) { return func( pipe ); } );
		Apply2AllOutPipesB( [&]( Pipe * const pipe ) { return func( pipe ); } );
	}

	virtual void MoveShape( MicroMeterPoint const & );

	virtual void DrawNeuronText( PixelCoordsFp & ) const;

protected:

	using PipeList = vector<Pipe *>;

	PipeList m_incoming;
	PipeList m_outgoing;

	void drawCircle( PixelCoordsFp const &, D2D1::ColorF const, MicroMeterPoint const, MicroMeter const ) const;
	void drawCircle( PixelCoordsFp const &, D2D1::ColorF const, MicroMeter const ) const;

	PixelRect const GetPixRect4Text( PixelCoordsFp const & ) const;
	void      const DisplayText( PixelRect const, wstring const ) const;

private:

	void addPipe      ( PipeList &, Pipe * const );
	void removePipe   ( PipeList &, Pipe * const );
	void clearPipeList( PipeList & );

	void apply2All( PipeList const & pipeList, PipeFunc const & func )
	{
		LockShape();
		for ( Pipe * pPipe : pipeList ) 
		{ 
			if ( pPipe != nullptr )
				func( pPipe );
		}
		UnlockShape();
	}

	bool apply2AllB( PipeList  const & pipeList, PipeFuncB const & func )
	{
		bool bResult { false };
		LockShape();
		for ( auto pipe : pipeList ) 
		{ 
			if ( pipe != nullptr )
			{
				bResult = func( pipe );
				if ( bResult )
					break;
			}
		}
		UnlockShape();
		return bResult;
	}

	MicroMeterPoint     m_center;
	MicroMeter          m_extension;
	IDWriteTextFormat * m_pTextFormat { nullptr };
};

BaseKnot const * Cast2BaseKnot( Shape const * );
BaseKnot       * Cast2BaseKnot( Shape       * );
