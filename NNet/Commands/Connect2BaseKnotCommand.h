// Connect2BaseKnotCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "ShapeId.h"
#include "Command.h"
#include "BaseKnot.h"

using std::swap;

class Connect2BaseKnotCommand : public Command
{
public:
	Connect2BaseKnotCommand
	( 
		BaseKnot * pBaseKnotSrc,
		BaseKnot * pBaseKnotDst
	)
	  :	m_pBaseKnotSrc( pBaseKnotSrc ),
		m_pBaseKnotDst( pBaseKnotDst )
	{ 
		if ( m_pBaseKnotDst->IsKnot() ) // if a Neuron is connected to a Knot, the Knot would survive
			swap( m_pBaseKnotDst, m_pBaseKnotSrc ); // swap makes sure, that the Neuron survives
		m_upDstConnections = m_pBaseKnotDst->m_connections.Clone();
	}

	~Connect2BaseKnotCommand( )	{ }

	virtual void Do( NNetModelWriterInterface & nmwi )
	{

		m_upBaseKnotSrc = nmwi.RemoveFromModel<BaseKnot>( * m_pBaseKnotSrc ); 
		assert( m_upBaseKnotSrc );
		m_pBaseKnotDst->AddConnections( m_upBaseKnotSrc.get() ); // double connections?
	}

	virtual void Undo( NNetModelWriterInterface & nmwi )
	{
		m_pBaseKnotDst->SetConnections( m_upDstConnections.get() );  // restore dst connections
		assert( m_upBaseKnotSrc );
		m_upBaseKnotSrc->Reconnect( );
		m_upBaseKnotSrc = nmwi.ReplaceInModel<BaseKnot,BaseKnot>( move( m_upBaseKnotSrc ) ); // reconnect src  
	}

private:

	BaseKnot * m_pBaseKnotSrc;
	BaseKnot * m_pBaseKnotDst;

	unique_ptr<BaseKnot>    m_upBaseKnotSrc    { nullptr };
	unique_ptr<Connections> m_upDstConnections { nullptr };
};
