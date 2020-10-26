// DeletePipeCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "ShapeId.h"
#include "Command.h"
#include "BaseKnot.h"
#include "Pipe.h"

using std::wcout;
using std::endl;

class DeletePipeCommand : public Command
{
public:

	DeletePipeCommand
	( 
		NNetModelWriterInterface & nmwi, 
		ShapeId              const idPipe 
	)
		: m_pPipe( nmwi.GetShapePtr<Pipe *>( idPipe ) )
	{
		wcout << L"DeletePipeCommand " << L"shapeId = " << m_pPipe->GetId( ) << endl;
		m_pPipe->CheckShape();
		m_pStartKnot = m_pPipe->GetStartKnotPtr();
		m_pEndKnot   = m_pPipe->GetEndKnotPtr();
	}

	~DeletePipeCommand( ){ }

	virtual void Do( NNetModelWriterInterface & nmwi )
	{
		wcout << L"DeletePipeCommand " << L"Do " << L"shapeId = " << m_pPipe->GetId( ) << endl;
		m_pPipe->CheckShape();
		m_pStartKnot->m_connections.RemoveOutgoing( m_pPipe );
		if ( m_pStartKnot->IsOrphanedKnot( ) )
			m_upStartKnot = move(nmwi.RemoveFromModel<Knot>( m_pStartKnot->GetId() ));

		m_pEndKnot->m_connections.RemoveIncoming( m_pPipe );
		if ( m_pEndKnot->IsOrphanedKnot( ) )
			m_upEndKnot = move(nmwi.RemoveFromModel<Knot>( m_pEndKnot->GetId() ));

		m_upPipe = move(nmwi.RemoveFromModel<Pipe>( m_pPipe->GetId() ));
	}

	virtual void Undo( NNetModelWriterInterface & nmwi )
	{
		wcout << L"DeletePipeCommand " << L"Undo " << L"shapeId = " << m_pPipe->GetId( ) << endl;
		m_pStartKnot->m_connections.AddOutgoing( m_pPipe );
		m_pEndKnot  ->m_connections.AddIncoming( m_pPipe );
		if ( m_upStartKnot )
			nmwi.Store2Model<BaseKnot>( move(m_upStartKnot) );
		if ( m_upEndKnot )
			nmwi.Store2Model<BaseKnot>( move(m_upEndKnot) );
		nmwi.Store2Model<Pipe>( move(m_upPipe) );
		m_pPipe->CheckShape();
	}

private:
	Pipe     * m_pPipe;
	BaseKnot * m_pStartKnot;
	BaseKnot * m_pEndKnot;

	unique_ptr<Pipe>     m_upPipe;
	unique_ptr<BaseKnot> m_upStartKnot;
	unique_ptr<BaseKnot> m_upEndKnot;
};
