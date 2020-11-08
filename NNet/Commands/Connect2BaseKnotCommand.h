// Connect2BaseKnotCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "ShapeId.h"
#include "Command.h"
#include "BaseKnot.h"

class Connect2BaseKnotCommand : public Command
{
public:
	Connect2BaseKnotCommand
	( 
		ShapeId const idSrc,
		ShapeId const idDst 
	)
	  :	m_idSrc( idSrc ),
		m_idDst( idDst )
	{ }

	~Connect2BaseKnotCommand( )	{ }

	virtual void Do( NNetModelWriterInterface & nmwi )
	{
		//std::wcout << L"*** Before Do Connect2BaseKnot(" << m_idSrc << L")" << endl; 
		//nmwi.DumpModel();
		m_pBaseKnotDst     = nmwi.GetShapePtr<BaseKnot *>( m_idDst );
		m_upDstConnections = m_pBaseKnotDst->m_connections.Clone();
		m_upBaseKnotSrc    = nmwi.RemoveFromModel<BaseKnot>(m_idSrc); 
		assert( m_upBaseKnotSrc );
		m_pBaseKnotDst->AddConnections( m_upBaseKnotSrc.get() ); // double connections?
		//std::wcout << L"*** After Do Connect2BaseKnot(" << m_idSrc << L")" << endl; 
		//nmwi.DumpModel();
	}

	virtual void Undo( NNetModelWriterInterface & nmwi )
	{
		m_pBaseKnotDst->SetConnections( m_upDstConnections.get() );  // restore dst connections
		m_upBaseKnotSrc->Reconnect( );
		m_upBaseKnotSrc = nmwi.ReplaceInModel<BaseKnot,BaseKnot>( move( m_upBaseKnotSrc ) ); // reconnect src  
		//std::wcout << L"*** After Undo Connect2BaseKnot" << endl; 
		//nmwi.DumpModel();
	}

private:
	ShapeId const m_idSrc;
	ShapeId const m_idDst; 

	BaseKnot              * m_pBaseKnotDst     { nullptr };
	unique_ptr<BaseKnot>    m_upBaseKnotSrc    { nullptr };
	unique_ptr<Connections> m_upDstConnections { nullptr };
};
