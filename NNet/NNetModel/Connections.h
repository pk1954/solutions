// Connections.h
//
// NNetModel

#pragma once

#include <fstream>
#include <vector>
#include "scanner.h"
#include "Pipe.h"

using std::endl;
using std::wcout;
using std::vector;
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
	
	void Dump() const
	{
		wcout << L" in" << Pipe::OPEN_BRACKET;
		for (auto it : m_incoming) { wcout << it->GetId() << L' '; }
		wcout << Pipe::CLOSE_BRACKET;
		wcout << L" out" << Pipe::OPEN_BRACKET;
		for (auto it : m_outgoing) { wcout << it->GetId() << L' '; }
		wcout << Pipe::CLOSE_BRACKET;
	}

	void SetIncoming(Connections const & incoming) 
	{ 
		m_incoming = incoming.m_incoming; 
	}

	void SetOutgoing(Connections const & outgoing) 
	{ 
		m_outgoing = outgoing.m_outgoing; 
	}

	Pipe & GetFirstOutgoing() { return * m_outgoing.front(); }
	Pipe & GetFirstIncoming() { return * m_incoming.front(); }

	Pipe const & GetFirstOutgoing() const { return * m_outgoing.front(); }
	Pipe const & GetFirstIncoming() const { return * m_incoming.front(); }

	void AddIncoming(Pipe * const p) { m_incoming.push_back(p); }
	void AddOutgoing(Pipe * const p) { m_outgoing.push_back(p); }

	void RemoveIncoming( Pipe * const p ) 
	{ 
		auto res = find(begin(m_incoming), end(m_incoming), p);
		assert( res != end(m_incoming) );
		m_incoming.erase( res );
	}

	void RemoveOutgoing( Pipe * const p ) 
	{ 
		auto res = find(begin(m_outgoing), end(m_outgoing), p);
		assert( res != end(m_outgoing) );
		m_outgoing.erase( res );
	}

	void ReplaceIncoming(Pipe * const pDel, Pipe * const pAdd) { replace( m_incoming.begin(), m_incoming.end(), pDel, pAdd ); }
	void ReplaceOutgoing(Pipe * const pDel, Pipe * const pAdd) { replace( m_outgoing.begin(), m_outgoing.end(), pDel, pAdd ); }

	bool const HasIncoming() const { return ! m_incoming.empty(); }
	bool const HasOutgoing() const { return ! m_outgoing.empty(); }
	bool const HasConnection(Type const type = Type::all) const
	{
		if (type == Type::in)
			return HasIncoming();
		else if ( type == Type::out )
			return HasOutgoing();
		else
			return HasIncoming() || HasOutgoing();
	}

	size_t GetNrOfIncomingConnections() const { return m_incoming.size(); }
	size_t GetNrOfOutgoingConnections() const { return m_outgoing.size(); }
	size_t GetNrOfConnections()         const { return m_incoming.size() + m_outgoing.size(); }
	bool   IsOrphan()                   const { return ! HasConnection(Type::all); }

	void ClearIncoming() { m_incoming.clear(); }
	void ClearOutgoing() { m_outgoing.clear(); }

	void Apply2AllInPipes (PipeFunc const &f) const { for (auto it : m_incoming) { f(* it); } }
	void Apply2AllOutPipes(PipeFunc const &f) const { for (auto it : m_outgoing) { f(* it); } }

	void Apply2AllPipes( Type const type, PipeFunc const & func ) const
	{
		if ( type == Type::in )
			Apply2AllInPipes( func );
		else if ( type == Type::out )
			Apply2AllOutPipes( func );
		else
			Apply2AllConnectedPipes( func );
	}

	bool Apply2AllInPipesB (PipeCrit const &f) const 
	{ 
		for (auto it : m_incoming) 
		{ 
			if (f(* it))
				return true;
		}
		return false;
	}

	bool Apply2AllOutPipesB(PipeCrit const &f) const 
	{ 
		for (auto it : m_outgoing) 
		{ 
			if (f(* it))
				return true;
		}
		return false;
	}

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
		out << L" in";
		for (auto it : con.m_incoming) { out << * it; }
		out << L" out"; 
		for (auto it : con.m_outgoing) { out << * it; }
		return out;
	}

private:
	vector<Pipe *> m_incoming {};
	vector<Pipe *> m_outgoing {};
};
