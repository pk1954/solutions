// Connections.h
//
// NNetModel

#pragma once

#include "PipeList.h"

class Connections
{
public:
	Connections * const Clone( ) const 
	{
		Connections * pCopy { new Connections() };
		pCopy->m_incoming = m_incoming;
		pCopy->m_outgoing = m_outgoing;
		return pCopy;
	}
	
	Pipe & GetFirstOutgoing() { return m_outgoing.GetFirst(); }
	Pipe & GetFirstIncoming() { return m_incoming.GetFirst(); }

	Pipe const & GetFirstOutgoing() const { return m_outgoing.GetFirst(); }
	Pipe const & GetFirstIncoming() const { return m_incoming.GetFirst(); }

	void AddIncoming( Pipe * const p ) { m_incoming.AddPipe( p ); }
	void AddOutgoing( Pipe * const p ) { m_outgoing.AddPipe( p ); }

	void RemoveIncoming( Pipe * const p ) { m_incoming.RemovePipe( p ); }
	void RemoveOutgoing( Pipe * const p ) { m_outgoing.RemovePipe( p ); }

	void ReplaceIncoming( Pipe * const pDel, Pipe * const pAdd ) { m_incoming.ReplacePipe( pDel, pAdd ); }
	void ReplaceOutgoing( Pipe * const pDel, Pipe * const pAdd ) { m_outgoing.ReplacePipe( pDel, pAdd ); }

	bool   HasIncoming( )                const { return ! m_incoming.IsEmpty(); }
	bool   HasOutgoing( )                const { return ! m_outgoing.IsEmpty(); }
	size_t GetNrOfIncomingConnections( ) const { return m_incoming.Size(); }
	size_t GetNrOfOutgoingConnections( ) const { return m_outgoing.Size(); }
	size_t GetNrOfConnections( )         const { return m_incoming.Size() + m_outgoing.Size(); }
	bool   IsOrphan( )                   const { return m_incoming.IsEmpty() && m_outgoing.IsEmpty(); }

	void ClearIncoming( ) { m_incoming.ClearPipeList( ); }
	void ClearOutgoing( ) { m_outgoing.ClearPipeList( ); }

	void Apply2AllInPipes ( PipeFunc const & func ) const { m_incoming.Apply2AllPipesInList( func ); }
	void Apply2AllOutPipes( PipeFunc const & func ) const { m_outgoing.Apply2AllPipesInList( func ); }

	bool Apply2AllInPipesB ( PipeCrit const & func ) const { return m_incoming.Apply2AllPipesInListB( func ); }
	bool Apply2AllOutPipesB( PipeCrit const & func ) const { return m_outgoing.Apply2AllPipesInListB( func ); }

	void Apply2AllConnectedPipes( PipeFunc const & func )
	{
		Apply2AllInPipes ( [&]( Pipe & pipe ) { func( pipe ); } );
		Apply2AllOutPipes( [&]( Pipe & pipe ) { func( pipe ); } );
	}

	void Apply2AllConnectedPipesB( PipeCrit const & func )
	{
		Apply2AllInPipesB ( [&]( Pipe & pipe ) { return func( pipe ); } );
		Apply2AllOutPipesB( [&]( Pipe & pipe ) { return func( pipe ); } );
	}

	void Restore( BaseKnot * const pBaseKnot )
	{
		Apply2AllInPipes ( [&]( Pipe & pipe ) { pipe.SetEndKnot  ( pBaseKnot ); } );
		Apply2AllOutPipes( [&]( Pipe & pipe ) { pipe.SetStartKnot( pBaseKnot ); } );
	}

	void Add( Connections const & src )
	{
		src.Apply2AllInPipes ( [&]( Pipe & pipe ) { AddIncoming( & pipe ); } );
		src.Apply2AllOutPipes( [&]( Pipe & pipe ) { AddOutgoing( & pipe ); } );
	}

	void Recalc( )
	{
		Apply2AllConnectedPipes( [&]( Pipe & pipe ) { pipe.Recalc( ); } );
	}

private:
	PipeList m_incoming;
	PipeList m_outgoing;
};
