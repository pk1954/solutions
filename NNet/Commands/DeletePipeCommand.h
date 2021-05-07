// DeletePipeCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "ShapeId.h"
#include "Command.h"
#include "BaseKnot.h"
#include "Knot.h"
#include "Pipe.h"

using std::wcout;
using std::endl;

class DeletePipeCommand : public Command
{
public:

	DeletePipeCommand( ShapeId const idPipe )
		: m_idPipe( idPipe )
	{}

	~DeletePipeCommand(){ }

	virtual void Do( NNetModelWriterInterface & nmwi )
	{
		if ( ! m_pPipe )
		{
			m_pPipe      = nmwi.GetShapePtr<Pipe *>( m_idPipe );
			m_pStartKnot = m_pPipe->GetStartKnotPtr();
			m_pEndKnot   = m_pPipe->GetEndKnotPtr();
		}
		m_pPipe->Check();
		m_pStartKnot->m_connections.RemoveOutgoing( m_pPipe );
		if ( m_pStartKnot->IsOrphanedKnot() )
			m_upStartKnot = nmwi.RemoveFromModel<Knot>( * m_pStartKnot );

		m_pEndKnot->m_connections.RemoveIncoming( m_pPipe );
		if ( m_pEndKnot->IsOrphanedKnot() )
			m_upEndKnot = nmwi.RemoveFromModel<Knot>( * m_pEndKnot );

		m_upPipe = nmwi.RemoveFromModel<Pipe>( * m_pPipe );
	}

	virtual void Undo( NNetModelWriterInterface & nmwi )
	{
		m_pStartKnot->m_connections.AddOutgoing( m_pPipe );
		m_pEndKnot  ->m_connections.AddIncoming( m_pPipe );
		if ( m_upStartKnot )
			nmwi.ReplaceInModel<BaseKnot,BaseKnot>( move(m_upStartKnot) );
		if ( m_upEndKnot )
			nmwi.ReplaceInModel<BaseKnot,BaseKnot>( move(m_upEndKnot) );
		nmwi.ReplaceInModel<Pipe,Pipe>( move(m_upPipe) );
		m_pPipe->Check();
	}

private:
	Pipe     * m_pPipe      { nullptr };
	BaseKnot * m_pStartKnot { nullptr };
	BaseKnot * m_pEndKnot   { nullptr };

	unique_ptr<Pipe>     m_upPipe;
	unique_ptr<BaseKnot> m_upStartKnot;
	unique_ptr<BaseKnot> m_upEndKnot;

	ShapeId const m_idPipe;
};
