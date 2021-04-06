// Connections.h
//
// NNetModel

#pragma once

#include "scanner.h"
#include "Pipe.h"
#include "ShapePtrList.h"

using std::endl;
using std::unique_ptr;
using std::make_unique;

class Connections
{
public:
	enum class Type	{ in, out, all };

	using PipeFunc = function<void(Pipe &)>;
	using PipeCrit = function<bool(Pipe const &)>;

	unique_ptr<Connections> Clone() const 
	{
		unique_ptr<Connections> upCopy { make_unique<Connections>() };
		upCopy->m_incoming = m_incoming;
		upCopy->m_outgoing = m_outgoing;
		return move(upCopy);
	}
	
	Pipe & GetFirstOutgoing() { return m_outgoing.GetFirst(); }
	Pipe & GetFirstIncoming() { return m_incoming.GetFirst(); }

	Pipe const & GetFirstOutgoing() const { return m_outgoing.GetFirst(); }
	Pipe const & GetFirstIncoming() const { return m_incoming.GetFirst(); }

	void AddIncoming( Pipe * const p ) { m_incoming.Add( p ); }
	void AddOutgoing( Pipe * const p ) { m_outgoing.Add( p ); }

	void RemoveIncoming( Pipe * const p ) { m_incoming.Remove( p ); }
	void RemoveOutgoing( Pipe * const p ) { m_outgoing.Remove( p ); }

	void ReplaceIncoming( Pipe * const pDel, Pipe * const pAdd ) { m_incoming.Replace( pDel, pAdd ); }
	void ReplaceOutgoing( Pipe * const pDel, Pipe * const pAdd ) { m_outgoing.Replace( pDel, pAdd ); }

	bool const HasIncoming() const { return ! m_incoming.IsEmpty(); }
	bool const HasOutgoing() const { return ! m_outgoing.IsEmpty(); }
	bool const HasConnection(Type const type = Type::all) const
	{
		if (type == Type::in)
			return HasIncoming();
		else if ( type == Type::out )
			return HasOutgoing();
		else
			return HasIncoming() || HasOutgoing();
	}

	size_t GetNrOfIncomingConnections() const { return m_incoming.Size(); }
	size_t GetNrOfOutgoingConnections() const { return m_outgoing.Size(); }
	size_t GetNrOfConnections()         const { return m_incoming.Size() + m_outgoing.Size(); }
	bool   IsOrphan()                   const { return ! HasConnection(Type::all); }

	void ClearIncoming() { m_incoming.Clear(); }
	void ClearOutgoing() { m_outgoing.Clear(); }

	void Apply2AllInPipes (PipeFunc const &f) const { m_incoming.Apply2All(f); }
	void Apply2AllOutPipes(PipeFunc const &f) const { m_outgoing.Apply2All(f); }

	void Apply2AllPipes( Type const type, PipeFunc const & func ) const
	{
		if ( type == Type::in )
			Apply2AllInPipes( func );
		else if ( type == Type::out )
			Apply2AllOutPipes( func );
		else
			Apply2AllConnectedPipes( func );
	}

	bool Apply2AllInPipesB (PipeCrit const &f) const { return m_incoming.Apply2AllB(f); }
	bool Apply2AllOutPipesB(PipeCrit const &f) const { return m_outgoing.Apply2AllB(f); }

	void Apply2AllConnectedPipes( PipeFunc const & func ) const
	{
		Apply2AllInPipes ([&](Pipe &pipe) { func(pipe); });
		Apply2AllOutPipes([&](Pipe &pipe) { func(pipe); });
	}

	bool Apply2AllConnectedPipesB( PipeCrit const & func )
	{
		if (Apply2AllInPipesB ([&](Pipe const & pipe) { return func(pipe); }))
			return true;
		if (Apply2AllOutPipesB([&](Pipe const & pipe) { return func(pipe); }))
			return true;
		return false;
	}

	void Reconnect( BaseKnot * const pBaseKnot )
	{
		Apply2AllInPipes ( [&]( Pipe & pipe ) { pipe.SetEndKnot  ( pBaseKnot ); } );
		Apply2AllOutPipes( [&]( Pipe & pipe ) { pipe.SetStartKnot( pBaseKnot ); } );
	}

	void Add( Connections const & src )
	{
		src.Apply2AllInPipes ( [&]( Pipe & pipe ) { AddIncoming( & pipe ); } );
		src.Apply2AllOutPipes( [&]( Pipe & pipe ) { AddOutgoing( & pipe ); } );
	}

	void Recalc()
	{
		Apply2AllConnectedPipes( [&]( Pipe & pipe ) { pipe.Recalc(); } );
	}

	friend wostream & operator<< ( wostream & out, Connections const & con )
	{
		out << L" " << con.m_incoming << L" " << con.m_outgoing;
		return out;
	}

private:
	ShapePtrList<Pipe> m_incoming;
	ShapePtrList<Pipe> m_outgoing;
};
