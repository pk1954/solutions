// NNetModelWriterInterface.cpp
//
// NNetWindows

#include "stdafx.h"
#include "assert.h"
#include <unordered_map>
#include "PixelTypes.h"
#include "Analyzer.h"
#include "win32_util.h"
#include "win32_script.h"
#include "win32_sound.h"
#include "NNetModel.h"
#include "BaseKnot.h"
#include "NNetModelWriterInterface.h"

using std::wostream;
using std::wcout;
using std::endl;
using std::unordered_map;

using KnotList = vector<Knot *>;

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
			pModel->Restore2ShapeList( m_pNewPipe );
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
			pModel->Restore2ShapeList( m_pBaseKnotSrc );           // reconnect src  
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
			pModel->DoDeleteBaseKnot( m_pBaseKnot );
		}

		virtual void Undo( NNetModel * const pModel )
		{
			pModel->UndoDeleteBaseKnot( m_pBaseKnot );
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
			pModel->DoDeletePipe( m_pPipe );
		}

		virtual void Undo( NNetModel * const pModel )
		{
			pModel->UndoDeletePipe( m_pPipe );
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
		DisconnectCommand( NNetModel * pModel, ShapeId const id )
          :	m_pBaseKnot( pModel->GetShapePtr<BaseKnot *>( id ) )
		{ 
			MicroMeterPoint umPos { m_pBaseKnot->GetPosition() };
			m_pBaseKnot->m_connections.Apply2AllInPipes
			( 
				[&]( Pipe & pipe ) // every incoming Pipe needs a new end knot
				{ 
					Knot * pKnotNew { new Knot( umPos ) };
					pKnotNew->SetId( pModel->NewShapeListSlot() );
					pKnotNew->m_connections.AddIncoming( & pipe );
					m_endKnots.push_back( pKnotNew );
				} 
			);
			m_pBaseKnot->m_connections.Apply2AllOutPipes
			( 
				[&]( Pipe & pipe ) // every outgoing Pipe needs a new start knot
				{ 
					Knot * pKnotNew { new Knot( umPos ) };
					pKnotNew->SetId( pModel->NewShapeListSlot() );
					pKnotNew->m_connections.AddOutgoing( & pipe );
					m_startKnots.push_back( pKnotNew );
				} 
			);
		}

		~DisconnectCommand()
		{
			if ( m_pBaseKnot->IsKnot() )
				delete m_pBaseKnot;
		}

		virtual void Do( NNetModel * const pModel )
		{
			for ( Knot * pKnot : m_startKnots )
			{
				Pipe & pipeOut { pKnot->m_connections.GetFirstOutgoing() };
				pipeOut.SetStartKnot( pKnot );
				pipeOut.DislocateStartPoint( );
				pipeOut.Recalc( );
				pModel->Restore2ShapeList( pKnot );
			}
			m_pBaseKnot->m_connections.ClearIncoming();
			for ( Knot * pKnot : m_endKnots )
			{
				Pipe & pipeIn { pKnot->m_connections.GetFirstIncoming() };
				pipeIn.SetEndKnot( pKnot );
				pipeIn.DislocateEndPoint( );
				pipeIn.Recalc( );
				pModel->Restore2ShapeList( pKnot );
			}
			m_pBaseKnot->m_connections.ClearOutgoing();
			assert( m_pBaseKnot->m_connections.IsOrphan( ) );
			if ( m_pBaseKnot->IsKnot() )
				pModel->RemoveFromShapeList( m_pBaseKnot );
		}

		virtual void Undo( NNetModel * const pModel )
		{
			for ( Knot * pKnot : m_startKnots )
			{
				Pipe & pipeOut { pKnot->m_connections.GetFirstOutgoing() };
				pipeOut.SetStartKnot( m_pBaseKnot );
				pipeOut.Recalc( );
				pModel->RemoveFromShapeList( pKnot );
			}
			for ( Knot * pKnot : m_endKnots )
			{
				Pipe & pipeIn { pKnot->m_connections.GetFirstIncoming() };
				pipeIn.SetEndKnot( m_pBaseKnot );
				pipeIn.Recalc( );
				pModel->RemoveFromShapeList( pKnot );
			}
			pModel->Restore2ShapeList( m_pBaseKnot );
		}

	private:
		BaseKnot * m_pBaseKnot;
		KnotList   m_startKnots;
		KnotList   m_endKnots;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	m_CmdStack.NewCommand( new DisconnectCommand( m_pModel, id ) );
}

void NNetModelWriterInterface::Convert2Neuron( ShapeId const id )
{
	class Convert2NeuronCommand : public Command
	{
	public:
		Convert2NeuronCommand(  NNetModel * pModel, ShapeId const idInputNeuron )
			:	m_pInputNeuron( pModel->GetShapePtr<InputNeuron *>( idInputNeuron )  )
		{ 
			m_pNeuron = new Neuron( m_pInputNeuron->GetPosition( ) );
			m_pNeuron->m_connections = m_pNeuron->m_connections;
			m_pNeuron->SetId( m_pInputNeuron->GetId( ) );
		}

		~Convert2NeuronCommand( )
		{
			delete m_pNeuron;
		}

		virtual void Do( NNetModel * const pModel )
		{
			pModel->ReplaceInShapeList( m_pInputNeuron, m_pNeuron );
		}

		virtual void Undo( NNetModel * const pModel )
		{
			pModel->ReplaceInShapeList( m_pNeuron, m_pInputNeuron );
		}

	private:
		InputNeuron * m_pInputNeuron;
		Neuron      * m_pNeuron;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	m_CmdStack.NewCommand( new Convert2NeuronCommand( m_pModel, id ) );
}

void NNetModelWriterInterface::Convert2InputNeuron( ShapeId const id )
{
	class Convert2InputNeuronCommand : public Command
	{
	public:
		Convert2InputNeuronCommand( NNetModel * pModel, ShapeId const idNeuron )
			:	m_pNeuron( pModel->GetShapePtr<Neuron *>( idNeuron )  )
		{ 
			m_pInputNeuron = new InputNeuron( m_pNeuron->GetPosition( ) );
			m_pInputNeuron->m_connections = m_pNeuron->m_connections;
			m_pInputNeuron->SetId( m_pNeuron->GetId( ) );
		}

		~Convert2InputNeuronCommand( )
		{
			delete m_pInputNeuron;
		}

		virtual void Do( NNetModel * const pModel )
		{
			pModel->ReplaceInShapeList( m_pNeuron, m_pInputNeuron );
		}

		virtual void Undo( NNetModel * const pModel )
		{
			pModel->ReplaceInShapeList( m_pInputNeuron, m_pNeuron );
		}

	private:
		Neuron      * m_pNeuron;
		InputNeuron * m_pInputNeuron;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	m_CmdStack.NewCommand( new Convert2InputNeuronCommand( m_pModel, id ) );
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
	m_CmdStack.NewCommand( new ToggleStopOnTriggerCommand( id ) );
}

void NNetModelWriterInterface::SetPulseRate( ShapeId const id, fHertz const fNewValue )
{
	class SetPulseRateCommand : public Command
	{
	public:
		SetPulseRateCommand( InputNeuron * pInputNeuron, fHertz const fNewValue )
		  :	m_pInputNeuron( pInputNeuron ),
			m_fHertz( fNewValue )
		{ }

		virtual void Do( NNetModel * const pModel ) 
		{ 
			m_fHertz = m_pInputNeuron->SetPulseFrequency( m_fHertz );
			pModel->ClearModel( );
		}

	private:
		InputNeuron * const m_pInputNeuron;
		fHertz              m_fHertz;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << fNewValue << endl;
	m_CmdStack.NewCommand( new SetPulseRateCommand( m_pModel->GetShapePtr<InputNeuron *>( id ), fNewValue ) );
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

void NNetModelWriterInterface::SetTriggerSound( ShapeId const id, SoundDescr const & sound )
{
	class SetTriggerSoundCommand : public Command
	{
	public:
		SetTriggerSoundCommand
		( 
			NNetModel * const   pModel, 
			ShapeId     const   id, 
			SoundDescr  const & sound
		)
		  : m_pNeuron( pModel->GetShapePtr<Neuron *>( id ) ),
			m_sound( sound )
		{ }

		virtual void Do( NNetModel * const pModel ) 
		{ 
			m_sound = m_pNeuron->SetTriggerSound( m_sound );
			pModel->ClearModel( );
		}

	private:
		Neuron * const m_pNeuron;
		SoundDescr     m_sound;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << sound.m_bOn << L" " << sound.m_frequency << L" " << sound.m_duration << endl;

	m_CmdStack.NewCommand( new SetTriggerSoundCommand( m_pModel, id, sound ) );
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
	class MoveBaseKnotCommand : public Command
	{
	public:
		MoveBaseKnotCommand( BaseKnot * const pBaseKnot, MicroMeterPoint const & delta )
		  : m_pBaseKnot     ( pBaseKnot ),
			m_delta         ( delta ),
			m_posBaseKnotOld( pBaseKnot->GetPosition() )
		{}

		virtual void Do( NNetModel * const pModel ) 
		{ 
			m_pBaseKnot->SetPosition( m_posBaseKnotOld + m_delta );
		}

		virtual void Undo( NNetModel * const pModel ) 
		{ 
			m_pBaseKnot->SetPosition( m_posBaseKnotOld );
		}

	private:
		BaseKnot      * const m_pBaseKnot;
		MicroMeterPoint const m_delta;
		MicroMeterPoint const m_posBaseKnotOld;
	};

	class MovePipeCommand : public Command
	{
	public:
		MovePipeCommand( Pipe * const pPipe, MicroMeterPoint const & delta )
		  : m_pPipe ( pPipe ),
			m_delta         ( delta ),
			m_posStartKnotOld( pPipe->GetStartKnotPtr()->GetPosition() ),
			m_posEndKnotOld  ( pPipe->GetEndKnotPtr  ()->GetPosition())
		{ }

		virtual void Do( NNetModel * const pModel ) 
		{ 
			m_pPipe->GetStartKnotPtr()->SetPosition( m_posStartKnotOld + m_delta );
			m_pPipe->GetEndKnotPtr  ()->SetPosition( m_posEndKnotOld   + m_delta  );
		}

		virtual void Undo( NNetModel * const pModel ) 
		{ 
			m_pPipe->GetStartKnotPtr()->SetPosition( m_posStartKnotOld );
			m_pPipe->GetEndKnotPtr  ()->SetPosition( m_posEndKnotOld   );
		}

	private:
		Pipe          * const m_pPipe;
		MicroMeterPoint const m_delta;
		MicroMeterPoint const m_posStartKnotOld;
		MicroMeterPoint const m_posEndKnotOld;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << delta << endl;

	Shape * const pShape { m_pModel->GetShape( id ) };

	if ( pShape->IsPipe( ) )
		m_CmdStack.NewCommand( new MovePipeCommand    ( static_cast<Pipe     *>( pShape ), delta ) );
	else
		m_CmdStack.NewCommand( new MoveBaseKnotCommand( static_cast<BaseKnot *>( pShape ), delta ) );
}

void NNetModelWriterInterface::MoveSelection( MicroMeterPoint const & delta )
{
	class MoveSelectionCommand : public Command
	{
	public:
		MoveSelectionCommand( MicroMeterPoint const & delta )
			: m_delta( -delta )
		{ }

		virtual void Do( NNetModel * const pModel ) 
		{ 
			MicroMeterPoint const delta = - m_delta;
			pModel->Apply2AllSelected<BaseKnot>
			( 
				[&]( BaseKnot & knot ) 
				{ 
					knot.MoveShape( delta ); 
					knot.m_connections.Apply2AllConnectedPipes( [&](Pipe & pipe) { pipe.Recalc(); } );
				} 
			);
			m_delta = delta;
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
		AddOutgoing2KnotCommand( NNetModel * pModel, ShapeId const id, MicroMeterPoint const & pos )
		{ 
			m_pKnotNew = new Knot( pos );
			m_pStart   = pModel->GetShapePtr<BaseKnot *>( id );
			m_pPipe    = new Pipe( m_pStart, m_pKnotNew );
			m_pPipe->Recalc();
			m_pKnotNew->m_connections.AddIncoming( m_pPipe );
		}

		~AddOutgoing2KnotCommand( )
		{
			delete m_pKnotNew;
			delete m_pPipe;
		}

		virtual void Do( NNetModel * const pModel ) 
		{ 
			m_pStart->m_connections.AddOutgoing( m_pPipe );
			pModel->Add2ShapeList( m_pKnotNew );
			pModel->Add2ShapeList( m_pPipe );
		}

		virtual void Undo( NNetModel * const pModel ) 
		{ 
			m_pStart->m_connections.RemoveOutgoing( m_pPipe );
			pModel->RemoveFromShapeList( m_pKnotNew );
			pModel->RemoveFromShapeList( m_pPipe );
		}

		virtual void Redo( NNetModel * const pModel ) 
		{ 
			m_pStart->m_connections.AddOutgoing( m_pPipe );
			pModel->Restore2ShapeList( m_pKnotNew );
			pModel->Restore2ShapeList( m_pPipe );
		}

	private:
		Knot     * m_pKnotNew;
		BaseKnot * m_pStart;
		Pipe     * m_pPipe;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << pos << endl;

	m_CmdStack.NewCommand( new AddOutgoing2KnotCommand( m_pModel, id, pos ) );
}

void NNetModelWriterInterface::AddIncoming2Knot( ShapeId const id, MicroMeterPoint const & pos )
{
	class AddIncoming2KnotCommand : public Command
	{
	public:
		AddIncoming2KnotCommand( NNetModel * pModel, ShapeId const id, MicroMeterPoint const & pos )
		{ 
			m_pKnotNew = new Knot( pos );
			m_pEnd     = pModel->GetShapePtr<BaseKnot *>( id );
			m_pPipe    = new Pipe( m_pEnd, m_pKnotNew );
			m_pPipe->Recalc();
			m_pKnotNew->m_connections.AddOutgoing( m_pPipe );
		}

		~AddIncoming2KnotCommand( )
		{
			delete m_pKnotNew;
			delete m_pPipe;
		}

		virtual void Do( NNetModel * const pModel ) 
		{ 
			m_pEnd->m_connections.AddIncoming( m_pPipe );
			pModel->Add2ShapeList( m_pKnotNew );
			pModel->Add2ShapeList( m_pPipe );
		}

		virtual void Undo( NNetModel * const pModel ) 
		{ 
			m_pEnd->m_connections.RemoveIncoming( m_pPipe );
			pModel->RemoveFromShapeList( m_pKnotNew );
			pModel->RemoveFromShapeList( m_pPipe );
		}

		virtual void Redo( NNetModel * const pModel ) 
		{ 
			m_pEnd->m_connections.AddIncoming( m_pPipe );
			pModel->Restore2ShapeList( m_pKnotNew );
			pModel->Restore2ShapeList( m_pPipe );
		}

	private:
		Knot     * m_pKnotNew;
		BaseKnot * m_pEnd;
		Pipe     * m_pPipe;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << pos << endl;

	m_CmdStack.NewCommand( new AddIncoming2KnotCommand( m_pModel, id, pos ) );
}

void NNetModelWriterInterface::AddOutgoing2Pipe( ShapeId const id, MicroMeterPoint const & pos )
{
	class AddOutgoing2PipeCommand : public Command
	{
	public:
		AddOutgoing2PipeCommand( NNetModel * pModel, ShapeId const idPipe, MicroMeterPoint const & pos )
		{
			m_pPipeOld      = pModel->GetShapePtr<Pipe *>( idPipe );
			m_pStartKnotOld = m_pPipeOld->GetStartKnotPtr( );
			m_pKnotInsert   = new Knot( pos );
			m_pKnotOrtho    = new Knot( pos + pModel->OrthoVector( idPipe ) );
			m_pPipeOrtho    = new Pipe( m_pKnotInsert, m_pKnotOrtho );
			m_pPipeExt      = new Pipe( m_pStartKnotOld, m_pKnotInsert );
			m_pPipeOrtho->Recalc();
			m_pPipeExt->Recalc();

			m_pKnotInsert->m_connections.AddOutgoing( m_pPipeOrtho );
			m_pKnotOrtho ->m_connections.AddIncoming( m_pPipeOrtho );
			m_pKnotInsert->m_connections.AddIncoming( m_pPipeExt );
			m_pKnotInsert->m_connections.AddOutgoing( m_pPipeOld );
		}

		~AddOutgoing2PipeCommand( )
		{
			delete m_pKnotInsert;
			delete m_pKnotOrtho;
			delete m_pPipeOrtho;
			delete m_pPipeExt;
		}

		void swap( ) 
		{ 
			m_pStartKnotOld->m_connections.AddOutgoing   ( m_pPipeExt );
			m_pStartKnotOld->m_connections.RemoveOutgoing( m_pPipeOld );
			m_pPipeOld->SetStartKnot( m_pKnotInsert );
		}

		virtual void Do( NNetModel * const pModel ) 
		{ 
			swap( );
			pModel->Add2ShapeList( m_pKnotOrtho  );
			pModel->Add2ShapeList( m_pKnotInsert );
			pModel->Add2ShapeList( m_pPipeOrtho  );
			pModel->Add2ShapeList( m_pPipeExt    );
		}

		virtual void Undo( NNetModel * const pModel ) 
		{ 
			m_pStartKnotOld->m_connections.AddOutgoing   ( m_pPipeOld );
			m_pStartKnotOld->m_connections.RemoveOutgoing( m_pPipeExt );
			m_pPipeOld->SetStartKnot( m_pStartKnotOld );

			pModel->RemoveFromShapeList( m_pKnotOrtho  );
			pModel->RemoveFromShapeList( m_pKnotInsert );
			pModel->RemoveFromShapeList( m_pPipeOrtho  );
			pModel->RemoveFromShapeList( m_pPipeExt    );
		}

		virtual void Redo( NNetModel * const pModel ) 
		{ 
			swap( );
			pModel->Restore2ShapeList( m_pKnotOrtho  );
			pModel->Restore2ShapeList( m_pKnotInsert );
			pModel->Restore2ShapeList( m_pPipeOrtho  );
			pModel->Restore2ShapeList( m_pPipeExt    );
		}

	private:
		Pipe     * m_pPipeOld      { nullptr };
		Pipe     * m_pPipeExt      { nullptr };
		Pipe     * m_pPipeOrtho    { nullptr };
		BaseKnot * m_pStartKnotOld { nullptr };
		Knot     * m_pKnotInsert   { nullptr };
		Knot     * m_pKnotOrtho    { nullptr };
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << pos << endl;

	m_CmdStack.NewCommand( new AddOutgoing2PipeCommand( m_pModel, id, pos ) );
}

void NNetModelWriterInterface::AddIncoming2Pipe( ShapeId const id, MicroMeterPoint const & pos )
{
	class AddIncoming2PipeCommand : public Command
	{
	public:
		AddIncoming2PipeCommand(  NNetModel * pModel, ShapeId const idPipe, MicroMeterPoint const & pos )
		{
			m_pPipeOld      = pModel->GetShapePtr<Pipe *>( idPipe );
			m_pStartKnotOld = m_pPipeOld->GetStartKnotPtr( );
			m_pKnotInsert   = new Knot( pos );
			m_pKnotOrtho    = new Knot( pos - pModel->OrthoVector( idPipe ) );
			m_pPipeOrtho    = new Pipe( m_pKnotOrtho, m_pKnotInsert );
			m_pPipeExt      = new Pipe( m_pStartKnotOld, m_pKnotInsert );
			m_pPipeOrtho->Recalc();
			m_pPipeExt->Recalc();

			m_pKnotInsert->m_connections.AddIncoming( m_pPipeOrtho );
			m_pKnotOrtho ->m_connections.AddOutgoing( m_pPipeOrtho );
			m_pKnotInsert->m_connections.AddIncoming( m_pPipeExt );
			m_pKnotInsert->m_connections.AddOutgoing( m_pPipeOld );
		}

		~AddIncoming2PipeCommand( )
		{
			delete m_pKnotInsert;
			delete m_pKnotOrtho;
			delete m_pPipeOrtho;
			delete m_pPipeExt;
		}

		void swap( ) 
		{ 
			m_pStartKnotOld->m_connections.AddOutgoing   ( m_pPipeExt );
			m_pStartKnotOld->m_connections.RemoveOutgoing( m_pPipeOld );
			m_pPipeOld->SetStartKnot( m_pKnotInsert );
		}

		virtual void Do( NNetModel * const pModel ) 
		{ 
			swap( );
			pModel->Add2ShapeList( m_pKnotOrtho  );
			pModel->Add2ShapeList( m_pKnotInsert );
			pModel->Add2ShapeList( m_pPipeOrtho  );
			pModel->Add2ShapeList( m_pPipeExt    );
		}

		virtual void Undo( NNetModel * const pModel ) 
		{ 
			m_pStartKnotOld->m_connections.AddOutgoing   ( m_pPipeOld );
			m_pStartKnotOld->m_connections.RemoveOutgoing( m_pPipeExt );
			m_pPipeOld->SetStartKnot( m_pStartKnotOld );

			pModel->RemoveFromShapeList( m_pKnotOrtho  );
			pModel->RemoveFromShapeList( m_pKnotInsert );
			pModel->RemoveFromShapeList( m_pPipeOrtho  );
			pModel->RemoveFromShapeList( m_pPipeExt    );
		}

		virtual void Redo( NNetModel * const pModel ) 
		{ 
			swap( );
			pModel->Restore2ShapeList( m_pKnotOrtho  );
			pModel->Restore2ShapeList( m_pKnotInsert );
			pModel->Restore2ShapeList( m_pPipeOrtho  );
			pModel->Restore2ShapeList( m_pPipeExt    );
		}

	private:
		Pipe     * m_pPipeOld      { nullptr };
		Pipe     * m_pPipeExt      { nullptr };
		Pipe     * m_pPipeOrtho    { nullptr };
		BaseKnot * m_pStartKnotOld { nullptr };
		Knot     * m_pKnotInsert   { nullptr };
		Knot     * m_pKnotOrtho    { nullptr };
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << pos << endl;

	m_CmdStack.NewCommand( new AddIncoming2PipeCommand( m_pModel, id, pos ) );
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
			m_pPipeNew->Recalc();
		}

		~InsertNeuronCommand( )
		{
			delete m_pNeuron;
			delete m_pPipeNew;
		}

	    void swap( ) 
		{ 
			m_pStartKnot->m_connections.RemoveOutgoing( m_pPipe2Split );
			m_pStartKnot->m_connections.AddOutgoing   ( m_pPipeNew );
			m_pPipe2Split->SetStartKnot( m_pNeuron );
		}

		virtual void Do( NNetModel * const pModel ) 
		{ 
			swap( );
			pModel->Add2ShapeList( m_pNeuron );
			pModel->Add2ShapeList( m_pPipeNew );
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

		virtual void Redo( NNetModel * const pModel ) 
		{ 
			swap( );
			pModel->Restore2ShapeList( m_pNeuron );
			pModel->Restore2ShapeList( m_pPipeNew );
		}

	private:
		Pipe     * m_pPipe2Split { nullptr };
		Pipe     * m_pPipeNew    { nullptr };
		Neuron   * m_pNeuron     { nullptr };
		BaseKnot * m_pStartKnot  { nullptr };
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue( ) << L" "<< pos << endl;

	m_CmdStack.NewCommand( new InsertNeuronCommand( m_pModel, id, pos ) );
}

void NNetModelWriterInterface::NewNeuron( MicroMeterPoint const & pos )
{
	class NewNeuronCommand : public Command
	{
	public:
		NewNeuronCommand( MicroMeterPoint const & pos )
		{ 
			m_pNeuron = new Neuron( pos );
		}

		~NewNeuronCommand( )
		{ 
			delete m_pNeuron;
		}

		virtual void Do( NNetModel * const pModel ) 
		{ 
			pModel->Add2ShapeList( m_pNeuron );
		}

		virtual void Undo( NNetModel * const pModel ) 
		{ 
			pModel->RemoveFromShapeList( m_pNeuron );
		}

		virtual void Redo( NNetModel * const pModel ) 
		{ 
			pModel->Restore2ShapeList( m_pNeuron );
		}

	private:
		Neuron * m_pNeuron;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << pos << endl;

	m_CmdStack.NewCommand( new NewNeuronCommand( pos ) );
}

void NNetModelWriterInterface::NewInputNeuron( MicroMeterPoint const & pos )
{
	class NewInputNeuronCommand : public Command
	{
	public:
		NewInputNeuronCommand( MicroMeterPoint const & pos )
		{ 
			m_pInputNeuron = new InputNeuron( pos );
		}

		~NewInputNeuronCommand( )
		{ 
			delete m_pInputNeuron;
		}

		virtual void Do( NNetModel * const pModel ) 
		{ 
			pModel->Add2ShapeList( m_pInputNeuron );
		}

		virtual void Undo( NNetModel * const pModel ) 
		{ 
			pModel->RemoveFromShapeList( m_pInputNeuron );
		}

		virtual void Redo( NNetModel * const pModel ) 
		{ 
			pModel->Restore2ShapeList( m_pInputNeuron );
		}

	private:
		InputNeuron * m_pInputNeuron;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << pos << endl;

	m_CmdStack.NewCommand( new NewInputNeuronCommand( pos ) );
}

void NNetModelWriterInterface::AppendNeuron( ShapeId const id )
{
	class AppendNeuronCommand : public Command
	{
	public:
		AppendNeuronCommand( NNetModel * pModel, ShapeId const id )
		{ 
			m_pKnot   = pModel->GetShapePtr<Knot *>( id );
			m_pNeuron = new Neuron( m_pKnot->GetPosition( ) );
			m_pNeuron->m_connections = m_pKnot->m_connections;
		}

		~AppendNeuronCommand( )
		{
			delete m_pNeuron;
		}

		virtual void Do( NNetModel * const pModel ) 
		{ 
			pModel->ReplaceInShapeList( m_pKnot, m_pNeuron );
		}

		virtual void Undo( NNetModel * const pModel ) 
		{ 
			pModel->ReplaceInShapeList( m_pNeuron, m_pKnot );
		}

	private:
		Neuron * m_pNeuron;
		Knot   * m_pKnot;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;

	m_CmdStack.NewCommand( new AppendNeuronCommand( m_pModel, id ) );
}

void NNetModelWriterInterface::AppendInputNeuron( ShapeId const id )
{
	class AppendInputNeuronCommand : public Command
	{
	public:
		AppendInputNeuronCommand( NNetModel * pModel, ShapeId const id )
		{ 
			m_pKnot        = pModel->GetShapePtr<Knot *>( id );
			m_pInputNeuron = new InputNeuron( m_pKnot->GetPosition( ) );
			m_pInputNeuron->m_connections = m_pKnot->m_connections;
		}

		~AppendInputNeuronCommand( )
		{
			delete m_pInputNeuron;
		}

		virtual void Do( NNetModel * const pModel ) 
		{ 
			pModel->ReplaceInShapeList( m_pKnot, m_pInputNeuron );
		}

		virtual void Undo( NNetModel * const pModel ) 
		{ 
			pModel->ReplaceInShapeList( m_pInputNeuron, m_pKnot );
		}

	private:
		InputNeuron * m_pInputNeuron;
		Knot        * m_pKnot;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;

	m_CmdStack.NewCommand( new AppendInputNeuronCommand( m_pModel, id ) );
}

void NNetModelWriterInterface::ClearBeepers( )
{
	class ClearBeepersCommand : public Command
	{
	public:
		ClearBeepersCommand( NNetModel * pModel )
		{
			pModel->Apply2All<Neuron>
			( 
				[&]( Neuron & neuron ) 
				{ 
					if ( neuron.HasTriggerSound() )
						m_beepers.push_back( Beeper{ & neuron, neuron.GetTriggerSound() } );
				} 
			);
		}

		void clearTriggerSound( Neuron * const pNeuron )
		{
			static const SoundDescr noSound { false, 0_Hertz, 0_MilliSecs };
			pNeuron->SetTriggerSound( noSound );
		}

		void clearAll( NNetModel * const pModel )
		{
			pModel->Apply2All<Neuron>( [&](Neuron & n) { clearTriggerSound( & n ); } );
		}

		void clearAllSelected( NNetModel * const pModel )
		{
			pModel->Apply2AllSelected<Neuron>( [&](Neuron & n) { clearTriggerSound( & n ); } );
		}

		virtual void Do( NNetModel * const pModel ) 
		{ 
			if ( pModel->AnyShapesSelected() )
				clearAllSelected( pModel );
			else
				clearAll( pModel );
		}

		virtual void Undo( NNetModel * const pModel ) 
		{ 
			clearAll( pModel );
			for ( Beeper const & beeper : m_beepers )
				beeper.m_pNeuron->SetTriggerSound( beeper.m_sound );
		}

	private:
		struct Beeper
		{
			Neuron   * m_pNeuron;
			SoundDescr m_sound;
		};
		vector<Beeper> m_beepers;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;

	m_CmdStack.NewCommand( new ClearBeepersCommand( m_pModel ) );
}

void NNetModelWriterInterface::MarkSelection( tBoolOp const op )
{
	class MarkSelectionCommand : public Command
	{
	public:
		MarkSelectionCommand( NNetModel * pModel, tBoolOp const op )
			: m_op( op )
		{ 
			m_markedShapes.clear();
			pModel->Add2ShapeList( m_markedShapes, [&]( Shape const & s ){ return s.IsMarked(); } );
		}

		virtual void Do( NNetModel * const pModel ) 
		{ 
			pModel->Apply2AllSelected<Shape>( [&]( Shape & shape ) { shape.Mark( m_op ); } );
		}

		virtual void Undo( NNetModel * const pModel ) 
		{ 
			pModel->Apply2All<Shape>( [&]( Shape & shape ) { shape.Mark( tBoolOp::opFalse ); } );
			for ( Shape * pShape : m_markedShapes )
				pShape->Mark( tBoolOp::opTrue );
		}

	private:
		ShapeList     m_markedShapes;
		tBoolOp const m_op;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;

	m_CmdStack.NewCommand( new MarkSelectionCommand( m_pModel, op ) );
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

///////////////////// selection commands /////////////////////////////

class SelectionCommand : public Command
{
public:
	SelectionCommand( NNetModel * const pModel )
	{ 
		m_selectedShapes.clear();
		pModel->Add2ShapeList( m_selectedShapes, [&]( Shape const & s ){ return s.IsSelected(); } );
	}

	virtual void Undo( NNetModel * const pModel ) 
	{
		pModel->SelectAll( tBoolOp::opFalse );
		for ( Shape * pShape : m_selectedShapes )
			pShape->Select( tBoolOp::opTrue );
	}

private:
	ShapeList m_selectedShapes;
};

void NNetModelWriterInterface::CopySelection( )
{
	class CopySelectionCommand : public SelectionCommand
	{
	public:

		CopySelectionCommand( NNetModel * const pModel )
			:	SelectionCommand( pModel)
		{ 
			// vector like m_Shapes with ptr to copy of shape or nullptr (if original shape is not in selection)
			ShapeList newShapes( pModel->GetSizeOfShapeList(), nullptr ); 

			pModel->Apply2AllSelected<Shape>  // make shallow copy of all selected shapes
			( 
				[&]( Shape const & shape ) 
				{ 
					newShapes.at(shape.GetId().GetValue()) = pModel->ShallowCopy( shape ); 
				} 
			);

			pModel->Apply2AllSelected<Shape>          // interconnect new shapes in same way as originals
			( 
				[&]( Shape const & shape ) 
				{ 
					pModel->ConnectToNewShapes( shape, newShapes ); 
				} 
			);
	
			pModel->SelectAll( tBoolOp::opFalse );  // Deselect original shapes. The copies inherited selection state at shallow copy

			for ( auto pShape : newShapes )         // Now add copies to m_Shapes and remember them in m_copies
			{ 
				if ( pShape )
				{
					pModel->Add2ShapeList( pShape );
					m_copies.push_back( pShape );
				}
			}
		}

		virtual void Do( NNetModel * const pModel ) 
		{ }

		virtual void Undo( NNetModel * const pModel ) 
		{ 
			for ( Shape * pShape : m_copies )             // disconnect copies
				pModel->RemoveFromShapeList( pShape );
			SelectionCommand::Undo( pModel );             // restore original selection
		}

		virtual void Redo( NNetModel * const pModel ) 
		{ 
			pModel->SelectAll( tBoolOp::opFalse );        // deselect all
			for ( Shape * pShape : m_copies )             // restore copies
				pModel->Restore2ShapeList( pShape );
		}

	private:
		ShapeList m_copies;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;

	m_CmdStack.NewCommand( new CopySelectionCommand( m_pModel ) );
}

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
			m_pShape( pModel->GetShapePtr<Shape *>( id ) ),
			m_op( op )
		{ }

		virtual void Do( NNetModel * const pModel ) 
		{ 
			m_pShape->Select( m_op ); 
		}

	private:
		Shape * m_pShape;
		tBoolOp m_op;
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
			m_pBaseKnot( pModel->GetShapePtr<BaseKnot *>( id ) ),
			m_op( op )
		{ }

		virtual void Do( NNetModel * const pModel ) 
		{ 
			pModel->SelectSubtree( m_pBaseKnot, m_op );
		}

	private:
		BaseKnot    * m_pBaseKnot;
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
			pModel->Apply2AllInRect<Shape>( m_rect, [&]( Shape & shape ) { shape.Select( tBoolOp::opTrue ); } );
		}

	private:
		MicroMeterRect const m_rect;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << rect << endl;

	m_CmdStack.NewCommand( new SelectShapesInRectCommand( m_pModel, rect ) );
}

void NNetModelWriterInterface::AnalyzeLoops( )
{
	class AnalyzeLoopsCommand : public SelectionCommand
	{
	public:
		AnalyzeLoopsCommand( NNetModel * const pModel )
			:	SelectionCommand( pModel)
		{ }

		virtual void Do( NNetModel * const pModel ) 
		{ 
			pModel->SelectAll( tBoolOp::opFalse );
			if ( ModelAnalyzer::FindLoop( * pModel ) )
				ModelAnalyzer::SelectLoopShapes( * pModel );
		}
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;

	m_CmdStack.NewCommand( new AnalyzeLoopsCommand( m_pModel ) );
}

void NNetModelWriterInterface::AnalyzeAnomalies( )
{
	class AnalyzeAnomaliesCommand : public SelectionCommand
	{
	public:
		AnalyzeAnomaliesCommand( NNetModel * const pModel )
		  :	SelectionCommand( pModel)
		{ }

		virtual void Do( NNetModel * const pModel ) 
		{ 
			pModel->SelectAll( tBoolOp::opFalse );
			if ( ModelAnalyzer::FindAnomaly( * pModel ) )
				ModelAnalyzer::SelectLoopShapes( * pModel );
		}
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;

	m_CmdStack.NewCommand( new AnalyzeAnomaliesCommand( m_pModel ) );
}
