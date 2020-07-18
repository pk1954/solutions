// NNetModelWriterInterface.cpp
//
// NNetWindows

#include "stdafx.h"
#include "assert.h"
#include <unordered_map>
#include "Resource.h"
#include "PixelTypes.h"
#include "Analyzer.h"
#include "win32_util.h"
#include "win32_script.h"
#include "NNetModel.h"
#include "BaseKnot.h"
#include "NNetModelWriterInterface.h"

using std::wostream;
using std::wcout;
using std::endl;
using std::unordered_map;

void NNetModelWriterInterface::Initialize( wostream * pTraceStream ) 
{ 
	m_pTraceStream = pTraceStream;
}

void NNetModelWriterInterface::Start( NNetModel * const pModel )
{
	m_pModel = pModel;
	m_CmdStack.Inizialize( m_pModel );
}

void NNetModelWriterInterface::Stop( )
{
	m_pModel = nullptr;
}

void NNetModelWriterInterface::UndoCommand( )
{
	if ( ! m_CmdStack.UndoCommand() )
		MessageBeep( MB_ICONWARNING );
}

void NNetModelWriterInterface::RedoCommand( )
{
	if ( ! m_CmdStack.RedoCommand() )
		MessageBeep( MB_ICONWARNING );
}

void NNetModelWriterInterface::ResetTimer( )
{
	class ResetTimerCommand : public Command
	{
	public:
		virtual void Do( NNetModel * const pModel ) 
		{ 
			m_simuTime = pModel->SetSimulationTime( m_simuTime); 
		}

	private:
		fMicroSecs m_simuTime { 0._MicroSecs };
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_CmdStack.NewCommand( new ResetTimerCommand() );
}

void NNetModelWriterInterface::Connect( ShapeId const idSrc, ShapeId const idDst )
{
	class Connect2PipeCommand : public Command
	{
	public:
		Connect2PipeCommand( BaseKnot * const pSrc, Pipe * const pDst )
		  :	m_pBaseKnot ( pSrc ),
			m_pPipe     ( pDst ),
			m_pNewPipe  ( nullptr ),
			m_pStartKnot( m_pPipe->GetStartKnotPtr( ) )
		{ 
			m_pNewPipe = new Pipe( m_pStartKnot, m_pBaseKnot );
		}

		~Connect2PipeCommand( )
		{ 
			delete m_pNewPipe;
		}

		virtual void Do( NNetModel * const pModel )
		{
			m_pStartKnot->m_connections.RemoveOutgoing( m_pPipe );
			m_pStartKnot->m_connections.AddOutgoing   ( m_pNewPipe );
			m_pBaseKnot ->m_connections.AddIncoming   ( m_pNewPipe );
			m_pBaseKnot ->m_connections.AddOutgoing   ( m_pPipe );
			m_pPipe->SetStartKnot( m_pBaseKnot );
			pModel->Add2ShapeList( m_pNewPipe );
			m_pPipe->Recalc();
		}

		virtual void Undo( NNetModel * const pModel )
		{
			m_pPipe->SetStartKnot( m_pStartKnot );
			m_pStartKnot->m_connections.RemoveOutgoing( m_pNewPipe );
			m_pStartKnot->m_connections.AddOutgoing( m_pPipe );
			pModel->RemoveFromShapeList( m_pNewPipe );
			m_pPipe->Recalc();
		}

		virtual void Redo( NNetModel * const pModel )
		{
			m_pStartKnot->m_connections.RemoveOutgoing( m_pPipe );
			m_pPipe->SetStartKnot( m_pBaseKnot );
			pModel->RestoreToShapeList( m_pNewPipe );
			m_pPipe->Recalc();
		}

	private:
		BaseKnot * m_pBaseKnot;
		Pipe     * m_pPipe;
		Pipe     * m_pNewPipe;
		BaseKnot * m_pStartKnot;
	};

	class Connect2BaseKnotCommand : public Command
	{
	public:
		Connect2BaseKnotCommand( BaseKnot * const pSrc, BaseKnot * const pDst )
		  :	m_pBaseKnotSrc( pSrc ),
			m_pBaseKnotDst( pDst ),
			m_pDstConnections( m_pBaseKnotDst->m_connections.Clone() )
		{ 
		}

		~Connect2BaseKnotCommand( )
		{
			delete m_pBaseKnotSrc;
			delete m_pDstConnections;
		}

		virtual void Do( NNetModel * const pModel )
		{
			m_pBaseKnotDst->AddConnections( m_pBaseKnotSrc );
			pModel->RemoveFromShapeList( m_pBaseKnotSrc ); // m_pBaseKnotSrc disconnected, but unchanged. Do not delete, will be reused in Undo
		}

		virtual void Undo( NNetModel * const pModel )
		{
			m_pBaseKnotDst->SetConnections( m_pDstConnections );  // restore dst connections
			m_pBaseKnotSrc->RestoreConnections( );
			pModel->RestoreToShapeList( m_pBaseKnotSrc );           // reconnect src  
		}

	private:
		BaseKnot    * m_pBaseKnotSrc;
		BaseKnot    * m_pBaseKnotDst;
		Connections * m_pDstConnections;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << idSrc.GetValue() << L" " << idDst.GetValue() << endl;
	BaseKnot * pSrc { m_pModel->GetShapePtr<BaseKnot *>( idSrc ) };
	Shape    * pDst { m_pModel->GetShapePtr<Shape    *>( idDst ) };
	Command  * pCommand;
	if ( pDst->IsPipe() )   // connect baseknot to pipe
		pCommand = new Connect2PipeCommand( pSrc, m_pModel->GetShapePtr<Pipe *>( idDst ) );
	else
		pCommand = new Connect2BaseKnotCommand( pSrc, m_pModel->GetShapePtr<BaseKnot *>( idDst ) );
	m_CmdStack.NewCommand( pCommand );
}

void NNetModelWriterInterface::DeleteShape( ShapeId const id )
{
	class DeleteBaseKnotCommand : public Command
	{
	public:
		DeleteBaseKnotCommand( BaseKnot * const pBaseKnot )
			: m_pBaseKnot( pBaseKnot )
		{ }

		~DeleteBaseKnotCommand( )
		{
			delete m_pBaseKnot;
		}

		virtual void Do( NNetModel * const pModel )
		{
			MicroMeterPoint umPos { m_pBaseKnot->GetPosition() };
			m_pBaseKnot->m_connections.Apply2AllInPipes
			( 
				[&]( Pipe & pipe ) // every incoming Pipe needs a new end knot
				{ 
					Knot * pKnotNew { pModel->NewShape<Knot>( umPos ) };
					pKnotNew->m_connections.AddIncoming( & pipe );
					pipe.SetEndKnot( pKnotNew );
					pipe.DislocateEndPoint( );
					pipe.Recalc();
				} 
			);
			m_pBaseKnot->m_connections.ClearIncoming();
			m_pBaseKnot->m_connections.Apply2AllOutPipes
			( 
				[&]( Pipe & pipe ) // every outgoing Pipe needs a new start knot
				{ 
					Knot * pKnotNew { pModel->NewShape<Knot>( umPos ) };
					pKnotNew->m_connections.AddOutgoing( & pipe );
					pipe.SetStartKnot( pKnotNew );
					pipe.DislocateStartPoint( );
					pipe.Recalc();
				} 
			);
			m_pBaseKnot->m_connections.ClearOutgoing();
			assert( m_pBaseKnot->m_connections.IsOrphan( ) );

			pModel->RemoveFromShapeList( m_pBaseKnot );
		}

		virtual void Undo( NNetModel * const pModel )
		{
			MicroMeterPoint umPos { m_pBaseKnot->GetPosition() };
			m_pBaseKnot->m_connections.Apply2AllInPipes
			( 
				[&]( Pipe & pipe ) // reconnect to original end knot
				{ 
					BaseKnot * pEndPoint { pipe.GetEndKnotPtr() };
					pipe.SetEndKnot( m_pBaseKnot );
					pipe.Recalc();
					delete pEndPoint;
				} 
			);
			m_pBaseKnot->m_connections.Apply2AllOutPipes
			( 
				[&]( Pipe & pipe ) // reconnect to original start knot
				{ 
					BaseKnot * pStartPoint { pipe.GetStartKnotPtr() };
					pipe.SetStartKnot( m_pBaseKnot );
					pipe.Recalc();
					delete pStartPoint;
				} 
			);
			pModel->RestoreToShapeList( m_pBaseKnot );
		}

	private:
		BaseKnot * m_pBaseKnot;
	};

	class DeletePipeCommand : public Command
	{
	public:
		DeletePipeCommand( Pipe * const pPipe )
		  : m_pPipe     ( pPipe ),
			m_pStartKnot( pPipe->GetStartKnotPtr() ),
			m_pEndKnot  ( pPipe->GetEndKnotPtr() )
		{ }

		~DeletePipeCommand( )
		{
			delete m_pStartKnot;                       
			delete m_pEndKnot;
			delete m_pPipe;
		}

		virtual void Do( NNetModel * const pModel )
		{
			m_pStartKnot->m_connections.RemoveOutgoing( m_pPipe );
			if ( m_pStartKnot->IsOrphanedKnot( ) && ( m_pStartKnot->IsKnot() ) )
				pModel->RemoveFromShapeList( m_pStartKnot );

			m_pEndKnot->m_connections.RemoveIncoming( m_pPipe );
			if ( m_pEndKnot->IsOrphanedKnot( ) && ( m_pEndKnot->IsKnot() ) )
				pModel->RemoveFromShapeList( m_pEndKnot );

			pModel->RemoveFromShapeList( m_pPipe );
		}

		virtual void Undo( NNetModel * const pModel )
		{
			m_pStartKnot->m_connections.AddOutgoing( m_pPipe );
			m_pEndKnot  ->m_connections.AddIncoming( m_pPipe );
			pModel->RestoreToShapeList( m_pStartKnot );
			pModel->RestoreToShapeList( m_pEndKnot );
			pModel->RestoreToShapeList( m_pPipe );
		}

	private:
		Pipe     * m_pPipe;
		BaseKnot * m_pStartKnot;
		BaseKnot * m_pEndKnot;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;

	Shape   * pShape { m_pModel->GetShapePtr<Shape *>( id ) };
	Command * pCommand;
	if ( pShape->IsPipe() )   // connect baseknot to pipe
		pCommand = new DeletePipeCommand( static_cast< Pipe * const >( pShape ) );
	else 
		pCommand = new DeleteBaseKnotCommand( static_cast< BaseKnot * const >( pShape ) );
	m_CmdStack.NewCommand( pCommand );
}

void NNetModelWriterInterface::Disconnect( ShapeId const id )
{
	class DisconnectCommand : public Command
	{
	public:
		DisconnectCommand( ShapeId const id )
			:	m_id( id )
		{ }

		virtual void Do( NNetModel * const pModel )
		{
			pModel->Disconnect( m_id );
		}

	private:
		ShapeId const m_id;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	m_CmdStack.NewCommand( new DisconnectCommand( id ) );
}

void NNetModelWriterInterface::Convert2Neuron( ShapeId const id )
{
	class Convert2NeuronCommand : public Command
	{
	public:
		Convert2NeuronCommand( ShapeId const id )
			:	m_id( id )
		{ }

		virtual void Do( NNetModel * const pModel )
		{
			pModel->Convert2Neuron( m_id );
		}

	private:
		ShapeId const m_id;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	m_CmdStack.NewCommand( new Convert2NeuronCommand( id ) );
}

void NNetModelWriterInterface::Convert2InputNeuron( ShapeId const id )
{
	class Convert2InputNeuronCommand : public Command
	{
	public:
		Convert2InputNeuronCommand( ShapeId const id )
			:	m_id( id )
		{ }

		virtual void Do( NNetModel * const pModel )
		{
			pModel->Convert2InputNeuron( m_id );
		}

	private:
		ShapeId const m_id;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	m_CmdStack.NewCommand( new Convert2InputNeuronCommand( id ) );
}

void NNetModelWriterInterface::ToggleStopOnTrigger( ShapeId const id )
{
	class ToggleStopOnTriggerCommand : public Command
	{
	public:
		ToggleStopOnTriggerCommand( ShapeId const id )
		  :	m_id( id )
		{ }

		virtual void Do( NNetModel * const pModel )
		{
			pModel->ToggleStopOnTrigger( m_id );
		}

	private:
		ShapeId const m_id;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	m_pModel->ToggleStopOnTrigger( id );
}

void NNetModelWriterInterface::SetPulseRate( ShapeId const id, fHertz const fNewValue )
{
	class SetPulseRateCommand : public Command
	{
	public:
		SetPulseRateCommand( ShapeId const id, fHertz const fNewValue )
		  :	m_id( id ),
			m_fHertz( fNewValue )
		{ }

		virtual void Do( NNetModel * const pModel ) 
		{ 
			m_fHertz = pModel->SetPulseRate( m_id, m_fHertz ); 
		}

	private:
		ShapeId const m_id;
		fHertz        m_fHertz;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << fNewValue << endl;
	m_CmdStack.NewCommand( new SetPulseRateCommand( id, fNewValue ) );
}

void NNetModelWriterInterface::DeleteSelection()
{
	class DeleteSelectionCommand : public Command
	{
	public:
		DeleteSelectionCommand( NNetModel * const pModel )
		{ 
		}

		virtual void Do( NNetModel * const pModel ) 
		{ 
			pModel->DeleteSelection();
		}

		virtual void Undo( NNetModel * const pModel ) 
		{
		}

	private:
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;

	m_CmdStack.NewCommand( new DeleteSelectionCommand( m_pModel ) );
}

void NNetModelWriterInterface::ResetModel( )
{
	class ResetModelCommand : public Command
	{
	public:
		ResetModelCommand( NNetModel * const pModel )
		{
			m_pModel = new NNetModel( * pModel ); // deep copy
		}

		~ResetModelCommand( )
		{
			delete m_pModel;
		}

		virtual void Do( NNetModel * const pModel )
		{ 
			pModel->ResetModel( );
			pModel->CreateInitialShapes();
		}

		virtual void Undo( NNetModel * const pModel ) 
		{
			* pModel = * m_pModel; // shallow assignment
		}

	private:
		NNetModel const * m_pModel;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_CmdStack.NewCommand( new ResetModelCommand( m_pModel ) );
}

void NNetModelWriterInterface::SetTriggerSound( ShapeId const id, bool const bActive, Hertz const freq, MilliSecs const ms )
{
	class SetTriggerSoundCommand : public Command
	{
	public:
		SetTriggerSoundCommand
		( 
			NNetModel * const pModel, 
			ShapeId     const id, 
			bool        const bActive, 
			Hertz       const freq, 
			MilliSecs   const ms 
		)
		  : m_pNeuron( pModel->GetShapePtr<Neuron *>( id ) ),
		    m_bActive( bActive ),
		    m_freq   ( freq ),
		    m_ms     ( ms )
		{ }

		virtual void Do( NNetModel * const pModel ) 
		{ 
			m_bActive = m_pNeuron->SetTriggerSoundOn       ( m_bActive );
			m_freq    = m_pNeuron->SetTriggerSoundFrequency( m_freq );
			m_ms      = m_pNeuron->SetTriggerSoundDuration ( m_ms );
			pModel->ClearModel( );
		}

	private:
		Neuron * const m_pNeuron;
		bool            m_bActive;
		Hertz           m_freq;
		MilliSecs       m_ms;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << bActive << L" " << freq << L" " << ms << endl;

	m_CmdStack.NewCommand( new SetTriggerSoundCommand( m_pModel, id, bActive, freq, ms ) );
}

void NNetModelWriterInterface::SetParameter( tParameter const param, float const fNewValue )
{
	class SetParameterCommand : public Command
	{
	public:
		SetParameterCommand( tParameter const param, float const fNewValue )
		  : m_param( param ),
			m_fValue( fNewValue )
		{ }

		virtual void Do( NNetModel * const pModel ) 
		{ 
			m_fValue = pModel->SetParameter( m_param, m_fValue ); 
		}

	private:
		tParameter const m_param;
		float            m_fValue;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << GetParameterName( param ) << L" " << fNewValue << endl;

	m_CmdStack.NewCommand( new SetParameterCommand( param, fNewValue ) );
}

void NNetModelWriterInterface::MoveShape( ShapeId const id, MicroMeterPoint const & delta )
{
	class MoveShapeCommand : public Command
	{
	public:
		MoveShapeCommand( ShapeId const id, MicroMeterPoint const & delta )
		  : m_id( id ),
			m_delta( -delta )
		{ }

		virtual void Do( NNetModel * const pModel ) 
		{ 
			pModel->MoveShape( m_id, m_delta = -m_delta); 
		}

	private:
		ShapeId   const m_id;
		MicroMeterPoint m_delta;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << delta << endl;
	m_CmdStack.NewCommand( new MoveShapeCommand( id, delta ) );
}

void NNetModelWriterInterface::MoveSelection( MicroMeterPoint const & delta )
{
	class MoveSelectionCommand : public Command
	{
	public:
		MoveSelectionCommand(  MicroMeterPoint const & delta )
			: m_delta( -delta )
		{ }

		virtual void Do( NNetModel * const pModel ) 
		{ 
			pModel->MoveSelection( m_delta = - m_delta); 
		}

	private:
		MicroMeterPoint m_delta;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << L" " << delta << endl;

	m_CmdStack.NewCommand( new MoveSelectionCommand( delta ) );
}

void NNetModelWriterInterface::AddOutgoing2Knot( ShapeId const id, MicroMeterPoint const & pos )
{
	class AddOutgoing2KnotCommand : public Command
	{
	public:
		AddOutgoing2KnotCommand( ShapeId const id, MicroMeterPoint const & pos )
			: m_id(id),
			  m_pos( pos )
		{ }

		virtual void Do( NNetModel * const pModel ) 
		{ 
			pModel->AddOutgoing2Knot( m_id, m_pos );
		}

	private:
		ShapeId         const m_id;
		MicroMeterPoint const m_pos;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << pos << endl;

	m_CmdStack.NewCommand( new AddOutgoing2KnotCommand( id, pos ) );
}

void NNetModelWriterInterface::AddIncoming2Knot( ShapeId const id, MicroMeterPoint const & pos )
{
	class AddIncoming2KnotCommand : public Command
	{
	public:
		AddIncoming2KnotCommand( ShapeId const id, MicroMeterPoint const & pos )
			: m_id(id),
			m_pos( pos )
		{ }

		virtual void Do( NNetModel * const pModel ) 
		{ 
			pModel->AddIncoming2Knot( m_id, m_pos );
		}

	private:
		ShapeId         const m_id;
		MicroMeterPoint const m_pos;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << pos << endl;

	m_CmdStack.NewCommand( new AddIncoming2KnotCommand( id, pos ) );
}

void NNetModelWriterInterface::AddOutgoing2Pipe( ShapeId const id, MicroMeterPoint const & pos )
{
	class AddOutgoing2PipeCommand : public Command
	{
	public:
		AddOutgoing2PipeCommand( ShapeId const id, MicroMeterPoint const & pos )
			: m_id(id),
			m_pos( pos )
		{ }

		virtual void Do( NNetModel * const pModel ) 
		{ 
			pModel->AddOutgoing2Pipe( m_id, m_pos );
		}

	private:
		ShapeId         const m_id;
		MicroMeterPoint const m_pos;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << pos << endl;

	m_CmdStack.NewCommand( new AddOutgoing2PipeCommand( id, pos ) );
}

void NNetModelWriterInterface::AddIncoming2Pipe( ShapeId const id, MicroMeterPoint const & pos )
{
	class AddIncoming2PipeCommand : public Command
	{
	public:
		AddIncoming2PipeCommand( ShapeId const id, MicroMeterPoint const & pos )
			: m_id(id),
			m_pos( pos )
		{ }

		virtual void Do( NNetModel * const pModel ) 
		{ 
			pModel->AddIncoming2Pipe( m_id, m_pos );
		}

	private:
		ShapeId         const m_id;
		MicroMeterPoint const m_pos;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << pos << endl;

	m_CmdStack.NewCommand( new AddIncoming2PipeCommand( id, pos ) );
}

void NNetModelWriterInterface::InsertNeuron( ShapeId const id, MicroMeterPoint const & pos )
{
	class InsertNeuronCommand : public Command
	{
	public:
		InsertNeuronCommand
		( 
			NNetModel     * const   pModel,
			ShapeId         const   id, 
			MicroMeterPoint const & umSplitPoint 
		)
		{ 
			m_pPipe2Split = pModel->GetShapePtr<Pipe *>( id );
			m_pStartKnot  = m_pPipe2Split->GetStartKnotPtr( );
			m_pNeuron     = new Neuron( umSplitPoint );
			m_pPipeNew    = new Pipe( m_pStartKnot, m_pNeuron );
			m_pNeuron->m_connections.AddOutgoing( m_pPipe2Split );
			m_pNeuron->m_connections.AddIncoming( m_pPipeNew );
		}

		~InsertNeuronCommand( )
		{
			delete m_pNeuron;
			delete m_pPipeNew;
		}

		virtual void Do( NNetModel * const pModel ) 
		{ 
			pModel->Add2ShapeList( m_pNeuron );
			pModel->Add2ShapeList( m_pPipeNew );
			m_pStartKnot->m_connections.RemoveOutgoing( m_pPipe2Split );
			m_pStartKnot->m_connections.AddOutgoing   ( m_pPipeNew );
			m_pPipe2Split->SetStartKnot( m_pNeuron );
			m_pPipeNew->Recalc();
		}

		virtual void Undo( NNetModel * const pModel ) 
		{ 
			pModel->RemoveFromShapeList( m_pNeuron );
			pModel->RemoveFromShapeList( m_pPipeNew );
			m_pStartKnot->m_connections.RemoveOutgoing( m_pPipeNew );
			m_pStartKnot->m_connections.AddOutgoing   ( m_pPipe2Split );
			m_pPipe2Split->SetStartKnot( m_pStartKnot );
			m_pPipeNew->Recalc();
		}

	private:
		Pipe     * m_pPipe2Split { nullptr };
		Pipe     * m_pPipeNew    { nullptr };
		Neuron   * m_pNeuron     { nullptr };
		BaseKnot * m_pStartKnot  { nullptr };
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << L" " << id.GetValue( ) << pos << endl;

	m_CmdStack.NewCommand( new InsertNeuronCommand( m_pModel, id, pos ) );
}

void NNetModelWriterInterface::NewNeuron( MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pModel->NewShape<Neuron>( pos );
}

void NNetModelWriterInterface::NewInputNeuron( MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pModel->NewShape<InputNeuron>( pos );
}

void NNetModelWriterInterface::AppendNeuron( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pModel->AppendShape<Neuron>( id );
}

void NNetModelWriterInterface::AppendInputNeuron( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pModel->AppendShape<InputNeuron>( id );
}

void NNetModelWriterInterface::ClearBeepers( )
{
	m_pModel->ClearBeepers( );
}

void NNetModelWriterInterface::MarkSelection( tBoolOp const op )
{
	m_pModel->MarkSelection( op );
}

void NNetModelWriterInterface::CopySelection( )
{
	m_pModel->CopySelection();
}

///////////////////// selection commands /////////////////////////////

class SelectionCommand : public Command
{
public:
	SelectionCommand( NNetModel * const pModel )
	{ 
		pModel->GetSelectionList( m_selectedShapes );
	}

	virtual void Undo( NNetModel * const pModel ) 
	{
		pModel->SetSelectionList( m_selectedShapes );
	}

private:
	ShapeList m_selectedShapes;
};

void NNetModelWriterInterface::SelectAllBeepers( )
{
	class SelectAllBeepersCommand : public SelectionCommand
	{
	public:
		SelectAllBeepersCommand( NNetModel * const pModel )
			:	SelectionCommand( pModel)
		{ }

		virtual void Do( NNetModel * const pModel ) 
		{ 
			pModel->SelectBeepers();
		}
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << endl;

	m_CmdStack.NewCommand( new SelectAllBeepersCommand( m_pModel ) );
}

void NNetModelWriterInterface::SelectShape( ShapeId const id, tBoolOp const op )
{
	class SelectShapeCommand : public SelectionCommand
	{
	public:
		SelectShapeCommand( NNetModel * const pModel, ShapeId const id, tBoolOp const op )
			:	SelectionCommand( pModel),
			m_id( id ),
			m_op( op )
		{ }

		virtual void Do( NNetModel * const pModel ) 
		{ 
			pModel->SelectShape( m_id, m_op ); 
		}

	private:
		ShapeId const m_id;
		tBoolOp const m_op;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << GetBoolOpName( op ) << endl;

	m_CmdStack.NewCommand( new SelectShapeCommand( m_pModel, id, op ) );
}

void NNetModelWriterInterface::SelectAll( tBoolOp const op )
{
	class SelectAllCommand : public SelectionCommand
	{
	public:
		SelectAllCommand( NNetModel * const pModel, tBoolOp const op )
			:	SelectionCommand( pModel),
			m_op( op )
		{ }

		virtual void Do( NNetModel * const pModel ) 
		{ 
			pModel->SelectAll( m_op ); 
		}

	private:
		tBoolOp m_op;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << GetBoolOpName( op ) << endl;

	m_CmdStack.NewCommand( new SelectAllCommand( m_pModel, op ) );
}

void NNetModelWriterInterface::SelectSubtree( ShapeId const id, tBoolOp const op )
{
	class SelectSubtreeCommand : public SelectionCommand
	{
	public:
		SelectSubtreeCommand( NNetModel * const pModel, ShapeId const id, tBoolOp const op )
			:	SelectionCommand( pModel),
			m_id( id ),
			m_op( op )
		{ }

		virtual void Do( NNetModel * const pModel ) 
		{ 
			pModel->SelectSubtree( m_id, m_op );
		}

	private:
		ShapeId const m_id;
		tBoolOp const m_op;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << GetBoolOpName( op ) << endl;

	m_CmdStack.NewCommand( new SelectSubtreeCommand( m_pModel, id, op ) );
}

void NNetModelWriterInterface::SelectShapesInRect( MicroMeterRect const & rect )
{
	class SelectShapesInRectCommand : public SelectionCommand
	{
	public:
		SelectShapesInRectCommand( NNetModel * const pModel, MicroMeterRect const & rect )
			:	SelectionCommand( pModel),
				m_rect( rect )
		{ }

		virtual void Do( NNetModel * const pModel ) 
		{ 
			pModel->SelectShapesInRect( m_rect );
		}

	private:
		MicroMeterRect const m_rect;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << rect << endl;

	m_CmdStack.NewCommand( new SelectShapesInRectCommand( m_pModel, rect ) );
}

void NNetModelWriterInterface::Analyze( int const cmd )
{
	class AnalyzeCommand : public SelectionCommand
	{
	public:
		AnalyzeCommand( NNetModel * const pModel, int const & cmd )
			:	SelectionCommand( pModel),
			    m_cmd( cmd )
		{ }

		virtual void Do( NNetModel * const pModel ) 
		{ 
			pModel->SelectAll( tBoolOp::opFalse );
			auto func { (m_cmd == IDM_ANALYZE_LOOPS) ? ModelAnalyzer::FindLoop : ModelAnalyzer::FindAnomaly };
			bool bFound { func( * pModel ) };
			if ( bFound )
				ModelAnalyzer::SelectLoopShapes( * pModel );
		}

	private:
		int m_cmd;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << L" " << cmd << endl;

	m_CmdStack.NewCommand( new AnalyzeCommand( m_pModel, cmd ) );
}
