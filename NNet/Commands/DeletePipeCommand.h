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
		NNetModelWriterInterface & model, 
		ShapeId              const idPipe 
	)
		: m_pPipe( model.GetShapePtr<Pipe *>( idPipe ) )
	{
		wcout << L"DeletePipeCommand " << L"shapeId = " << m_pPipe->GetId( ) << endl;
		m_pPipe->CheckShape();
		m_pStartKnot = m_pPipe->GetStartKnotPtr();
		m_pEndKnot   = m_pPipe->GetEndKnotPtr();
	}

	~DeletePipeCommand( ){ }

	virtual void Do( NNetModelWriterInterface & model )
	{
		wcout << L"DeletePipeCommand " << L"Do " << L"shapeId = " << m_pPipe->GetId( ) << endl;
		m_pPipe->CheckShape();
		m_pStartKnot->m_connections.RemoveOutgoing( m_pPipe );
		if ( m_pStartKnot->IsOrphanedKnot( ) )
			model.RemoveFromModel( m_pStartKnot );

		m_pEndKnot->m_connections.RemoveIncoming( m_pPipe );
		if ( m_pEndKnot->IsOrphanedKnot( ) )
			model.RemoveFromModel( m_pEndKnot );

		model.RemoveFromModel( m_pPipe );
	}

	virtual void Undo( NNetModelWriterInterface & model )
	{
		wcout << L"DeletePipeCommand " << L"Undo " << L"shapeId = " << m_pPipe->GetId( ) << endl;
		m_pStartKnot->m_connections.AddOutgoing( m_pPipe );
		m_pEndKnot  ->m_connections.AddIncoming( m_pPipe );
		model.Store2Model( m_pStartKnot );
		model.Store2Model( m_pEndKnot );
		model.Store2Model( m_pPipe );
		m_pPipe->CheckShape();
	}

private:
	Pipe     * m_pPipe;
	BaseKnot * m_pStartKnot;
	BaseKnot * m_pEndKnot;
};
