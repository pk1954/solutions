// DeletePipeCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "NobId.h"
#include "Command.h"
#include "BaseKnot.h"
#include "Knot.h"
#include "Pipe.h"

using std::wcout;
using std::endl;

class DeletePipeCommand : public Command
{
public:

	DeletePipeCommand(Nob &nob)
	  :	m_pipe(*Cast2Pipe(&nob))
	{}

	~DeletePipeCommand(){ }

	virtual void Do( NNetModelWriterInterface & nmwi )
	{
		BaseKnot & startKnot = * m_pipe.GetStartKnotPtr();
		startKnot.RemoveOutgoing( & m_pipe );
		if ( startKnot.IsOrphanedKnot() )
			m_upStartKnot = nmwi.RemoveFromModel<Knot>( startKnot );

		BaseKnot & endKnot = * m_pipe.GetEndKnotPtr();
		endKnot.RemoveIncoming( & m_pipe );
		if ( endKnot.IsOrphanedKnot() )
			m_upEndKnot = nmwi.RemoveFromModel<Knot>( endKnot );

		m_upPipe = nmwi.RemoveFromModel<Pipe>( m_pipe );
	}

	virtual void Undo( NNetModelWriterInterface & nmwi )
	{
		BaseKnot & startKnot = * m_pipe.GetStartKnotPtr();
		startKnot.AddOutgoing( & m_pipe );
		if ( m_upStartKnot )
			nmwi.Restore2Model( move(m_upStartKnot) );

		BaseKnot & endKnot = * m_pipe.GetEndKnotPtr();
		endKnot.AddIncoming( & m_pipe );
		if ( m_upEndKnot )
			nmwi.Restore2Model( move(m_upEndKnot) );

		nmwi.Restore2Model( move(m_upPipe) );
	}

private:
	Pipe               & m_pipe;
	unique_ptr<Pipe>     m_upPipe;
	unique_ptr<BaseKnot> m_upStartKnot;
	unique_ptr<BaseKnot> m_upEndKnot;
};
