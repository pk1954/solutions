// NNetModel.cpp 
//
// NNetModel

#include "stdafx.h"
#include <vector>
#include <unordered_map>
#include "MoreTypes.h"
#include "NNetParameters.h"
#include "tHighlightType.h"
#include "ShapeType.h"
#include "Knot.h"
#include "Neuron.h"
#include "InputNeuron.h"
#include "ComputeThread.h"
#include "NNetModel.h"

using namespace std::chrono;
using std::unordered_map;

NNetModel::NNetModel
(
	Param             * const pParam, 
	ObserverInterface * const pObserver 
)
  : m_pParam( pParam ),
	m_pChangeObserver( pObserver )
{					
	Shape::SetParam( pParam );
	m_Shapes.reserve( 100000 ); // Dirty trick to avoid reallocation (invalidates iterators)
}                               // TODO: find better solution  XXXXX

NNetModel::~NNetModel( ) { }

void NNetModel::CreateInitialShapes( )
{
	InputNeuron * const pInputNeuron { NewShape<InputNeuron >( MicroMeterPoint( 400.0_MicroMeter, 200.0_MicroMeter ) ) };
	Neuron      * const pNeuron      { NewShape<Neuron>      ( MicroMeterPoint( 400.0_MicroMeter, 800.0_MicroMeter ) ) };
	NewPipe( pInputNeuron, pNeuron );
}

void NNetModel::modelHasChanged( ) const 
{ 
	m_pChangeObserver->Notify( true );
}

void NNetModel::RecalcAllShapes( ) 
{ 
	Apply2All<Shape>( [&]( Shape & shape ) { shape.Recalc( ); } );
} 

void NNetModel::removeShape( Shape * const pShape )
{
	if ( pShape )
	{
		if ( pShape->IsPipe() )
			deletePipeEndPoints( Cast2Pipe( pShape ) );
		else 
			disconnectBaseKnot( Cast2BaseKnot( pShape ) );
		deleteShape( pShape );
	}
}

void NNetModel::Disconnect( ShapeId const id )
{
	if ( BaseKnot * pBaseKnot { GetShapePtr<BaseKnot *>( id ) } )
	{
		disconnectBaseKnot( pBaseKnot );
		if ( pBaseKnot->IsKnot() )
		{
			deleteShape( pBaseKnot );
			modelHasChanged( );
		}
	}
}

void NNetModel::Convert2Neuron( ShapeId const idInputNeuron )
{
	if ( InputNeuron * pInputNeuron { GetShapePtr<InputNeuron *>( idInputNeuron ) } )
	{
		ShapeId         const idAxon  { pInputNeuron->GetAxonId( ) };
		MicroMeterPoint const pos     { pInputNeuron->GetPosition( ) };
		Neuron        * const pNeuron { NewShape<Neuron>( pos ) };
		RemoveShape( idInputNeuron );
		if ( idAxon != NO_SHAPE )
			Connect( GetStartKnotId( idAxon ), pNeuron->GetId()  );
		modelHasChanged( );
	}
}

void NNetModel::Convert2InputNeuron( ShapeId const idNeuron )
{
	if ( Neuron * pNeuron { GetShapePtr<Neuron *>( idNeuron ) } )
	{
		ShapeId         const idAxon  { pNeuron->GetAxonId( ) };
		MicroMeterPoint const pos     { pNeuron->GetPosition( ) };
		InputNeuron   * const pInputNeuron { NewShape<InputNeuron>( pos ) };
		RemoveShape( idNeuron );
		if ( idAxon != NO_SHAPE )
			Connect( GetStartKnotId( idAxon ), pInputNeuron->GetId()  );
		modelHasChanged( );
	}
}

void NNetModel::SetPulseRate_Lock( ShapeId const id, float const fNewValue )
{
	InputNeuron * const pInputNeuron { GetShapePtr<InputNeuron *>( id ) };
	if ( pInputNeuron )
	{
		pInputNeuron->SetPulseFrequency_Lock( static_cast< fHertz >( fNewValue ) );
		modelHasChanged( );
	}
}

void NNetModel::setTriggerSound( Neuron * const pNeuron, Hertz const freq, MilliSecs const msec )
{
	if ( pNeuron )
	{
		pNeuron->SetTriggerSoundFrequency( freq );
		pNeuron->SetTriggerSoundDuration ( msec );
		pNeuron->SetTriggerSoundOn( freq != 0_Hertz );
		modelHasChanged( );
	}
}

void NNetModel::removeTriggerSound( Neuron * const pNeuron )
{
	setTriggerSound( pNeuron, 0_Hertz, 0_MilliSecs );
}

void NNetModel::SetTriggerSound( ShapeId const id, Hertz const freq, MilliSecs const msec )
{
	setTriggerSound( GetShapePtr<Neuron *>( id ), freq, msec );
}

void NNetModel::SetParameter
( 
	tParameter const param, 
	float      const fNewValue 
)
{
	m_pParam->SetParameterValue( param, fNewValue );
	RecalcAllShapes( );
	modelHasChanged( );
}

MicroMeterPoint const NNetModel::GetShapePos( ShapeId const id ) const 
{
	if ( BaseKnot const * pBaseKnot = GetShapeConstPtr<BaseKnot const *>( id ) )
		return pBaseKnot->GetPosition();
	return NP_NULL;
}

void NNetModel::Connect( ShapeId const idSrc, ShapeId const idDst )  // merge src shape into dst shape
{
	BaseKnot * pSrc { GetShapePtr<BaseKnot *>( idSrc ) };
	Shape    * pDst { GetShapePtr<Shape    *>( idDst ) };
	if ( pDst->TypeFits( ShapeType::Value::pipe ) )  // connect baseknot to pipe
	{
		insertBaseKnot( GetShapePtr<Pipe *>( idDst ), pSrc );
	}
	else  // connect baseknot to baseknot
	{
		BaseKnot * pDstBaseKnot { static_cast<BaseKnot *>( pDst ) };
		if ( pSrc && pDstBaseKnot )
		{
			pSrc->Apply2AllInPipes_Lock ( [&]( Pipe * const pPipe ) { ConnectIncoming( pPipe, pDstBaseKnot ); pPipe->Recalc(); } );
			pSrc->Apply2AllOutPipes_Lock( [&]( Pipe * const pPipe ) { ConnectOutgoing( pPipe, pDstBaseKnot ); pPipe->Recalc(); } );
			if ( pSrc->IsKnot() )
				deleteShape( pSrc );
		}
	}
	modelHasChanged( );
}

void NNetModel::NewPipe( BaseKnot * const pStart, BaseKnot * const pEnd )
{
	if ( pStart && pEnd )
	{
		Pipe * const pPipe { NewShape<Pipe>( NP_NULL ) };
		pPipe->LockShapeExclusive();
		ConnectOutgoing( pPipe, pStart );
		ConnectIncoming( pPipe, pEnd );
		pPipe->Recalc();
		pPipe->UnlockShapeExclusive();
		modelHasChanged( );
	}
}

void NNetModel::MoveShape( ShapeId const id, MicroMeterPoint const & delta )
{
	Shape * pShape { GetShapePtr<Shape *>( id  ) };
	if ( pShape )
	{
		if ( HasType<Pipe>( pShape ) )
		{
			Pipe * pPipe { static_cast<Pipe *>( pShape) };
			MoveShape( pPipe->GetStartKnotId(), delta );
			MoveShape( pPipe->GetEndKnotId  (), delta );
		}
		else 
		{
			BaseKnot * pBaseKnot { static_cast<BaseKnot *>( pShape ) };
			pBaseKnot->MoveShape( delta );
			pBaseKnot->Apply2AllConnectedPipes( [&](auto pipe) { pipe->Recalc(); } );
		}
		modelHasChanged( );
	}
}

Neuron * const NNetModel::InsertNeuron( ShapeId const idPipe, MicroMeterPoint const & splitPoint )
{
	Neuron * pNeuron { nullptr };
	if ( IsDefined( idPipe ) )
	{
		pNeuron = NewShape<Neuron>( splitPoint );
		insertBaseKnot( GetShapePtr<Pipe *>(idPipe), pNeuron );
		modelHasChanged( );
	}
	return pNeuron;
}

Knot * const NNetModel::InsertKnot( ShapeId const idPipe, MicroMeterPoint const & splitPoint )
{
	Knot * pKnot { nullptr };
	if ( IsDefined( idPipe ) )
	{
		pKnot = NewShape<Knot>( splitPoint );
		insertBaseKnot( GetShapePtr<Pipe *>(idPipe), pKnot );
		modelHasChanged( );
	}
	return pKnot;
}

void NNetModel::AddOutgoing2Pipe( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsDefined( id ) )
		NewPipe( InsertKnot( id, pos ), NewShape<Knot>( pos + orthoVector( id ) ) );
}

void NNetModel::AddIncoming2Pipe( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsDefined( id ) )
		NewPipe( NewShape<Knot>( pos - orthoVector( id ) ), InsertKnot( id, pos ) );
}

void NNetModel::AddOutgoing2Knot( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsDefined( id ) )
		NewPipe( GetShapePtr<BaseKnot *>(id), NewShape<Knot>( pos + STD_OFFSET) );
}

void NNetModel::AddIncoming2Knot( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsDefined( id ) )
		NewPipe( NewShape<Knot>( pos - STD_OFFSET ), GetShapePtr<BaseKnot *>( id ) );
}

void NNetModel::Compute( )
{
	Apply2All<Shape>( [&]( Shape & shape ) { shape.Prepare( ); } );
	Apply2All<Shape>( [&]( Shape & shape ) { shape.Step( ); } );
	m_timeStamp += m_pParam->GetTimeResolution( );
}

void NNetModel::ResetModel( )
{
	AcquireSRWLockExclusive( & m_SRWLockModel );
	for (auto pShape : m_Shapes)  // Do not use Apply2All! Locking.
		delete pShape;
	m_Shapes.clear();
	ReleaseSRWLockExclusive( & m_SRWLockModel );
	modelHasChanged( );
}

void NNetModel::DeleteSelection( )
{
	for ( int i = 0; i < m_Shapes.size(); ++i )  // Caution!
	{	                                         // Range based loop does not work here
		Shape * p = m_Shapes.at(i);              // removeShape changes the range 
		if ( p &&  p->IsSelected() )             // by creating new shapes!!
			removeShape( p ); 
	}
	modelHasChanged( );
}

void NNetModel::MarkSelection( tBoolOp const op )
{
	Apply2AllSelected<Shape>( [&]( Shape & shape ) { shape.Mark( op ); } );
}

void NNetModel::MoveSelection( MicroMeterPoint const & delta )
{
	Apply2AllSelected<BaseKnot>
	( 
		[&]( BaseKnot & knot ) 
		{ 
			knot.MoveShape( delta ); 
			knot.Apply2AllConnectedPipes( [&](auto pipe) { pipe->Recalc(); } );
		} 
	);
	modelHasChanged( );
}

Shape * NNetModel::shallowCopy( Shape const & shape ) // No locks
{
	switch ( shape.GetShapeType().GetValue() )
	{
	case ShapeType::Value::inputNeuron:
		return new InputNeuron( static_cast<InputNeuron const &>( shape ) );

	case ShapeType::Value::knot:
		return new Knot( static_cast<Knot const &>( shape ) );

	case ShapeType::Value::neuron:
		return new Neuron( static_cast<Neuron const &>( shape ) );

	case ShapeType::Value::pipe:
		return new Pipe( static_cast<Pipe const &>( shape ) );

	default:
		assert( false );
		return nullptr;
	}
}

// connectToNewShapes calls locking functions, but only destination shapes are locked
// therefore it's safe to call from Apply2All ...

void NNetModel::connectToNewShapes( Shape & shapeSrc, ShapeList & newShapes )
{ 
	Shape & shapeDst { * newShapes.at(shapeSrc.GetId().GetValue()) };
	if ( shapeSrc.IsPipe( ) )
	{
		Pipe & pipeSrc { static_cast<Pipe &>( shapeSrc ) };
		Pipe & pipeDst { static_cast<Pipe &>( shapeDst ) };
		pipeDst.LockShapeExclusive();
		pipeDst.SetStartKnot( static_cast<BaseKnot *>( newShapes.at( pipeSrc.GetStartKnotId().GetValue() )) );
		pipeDst.SetEndKnot  ( static_cast<BaseKnot *>( newShapes.at( pipeSrc.GetEndKnotId  ().GetValue() )) );
		pipeDst.Recalc();
		pipeDst.UnlockShapeExclusive();
	}
	else
	{
		BaseKnot & baseKnotSrc { static_cast<BaseKnot &>( shapeSrc ) };
		BaseKnot & baseKnotDst { static_cast<BaseKnot &>( shapeDst ) };
		baseKnotDst.ClearOutgoing();
		baseKnotDst.ClearIncoming();
		auto dstFromSrc = [&](Pipe * pPipeSrc){ return static_cast<Pipe *>(newShapes.at(pPipeSrc->GetId().GetValue())); };
		baseKnotSrc.Apply2AllOutPipes_Lock( [&]( Pipe * const pPipeSrc ) { baseKnotDst.AddOutgoing( dstFromSrc(pPipeSrc) ); } );
		baseKnotSrc.Apply2AllInPipes_Lock ( [&]( Pipe * const pPipeSrc ) { baseKnotDst.AddIncoming( dstFromSrc(pPipeSrc) ); } );
	}
}

void NNetModel::CopySelection( )
{
	// vector like m_Shapes with ptr to copy of shape or nullptr (if original shape is not in selection)
	ShapeList newShapes( m_Shapes.size(), nullptr ); 

	// make shallow copy of all selected shapes. Safe, no locks.
	Apply2AllSelected<Shape>( [&]( Shape & shape ) { newShapes.at(shape.GetId().GetValue()) = shallowCopy( shape ); } );

	// interconnect new shapes in same way as originals. Safe, no locks. 
	Apply2AllSelected<Shape>( [&]( Shape & shape ) { connectToNewShapes( shape, newShapes ); } ); // safe

	// Deselect original shapes, copies inherited selection at shallow copy
	SelectAll( tBoolOp::opFalse );

	// Now add copies to m_Shapes
	for ( auto pShape : newShapes )	{ if ( pShape ) add2ShapeList( pShape ); }

	modelHasChanged( );
}

/////////////////// local functions ///////////////////////////////////////////////

void NNetModel::selectSubtree( BaseKnot * const pBaseKnot, tBoolOp const op )
{
	if ( pBaseKnot )
	{
		pBaseKnot->Select( op );
		pBaseKnot->Apply2AllOutPipes_Lock
		( 
			[&]( Pipe * const pipe ) 
			{ 
				pipe->Select( op ); 
				BaseKnot * const pEndKnot { pipe->GetEndKnotPtr() };
				if ( pEndKnot->IsKnot() )
					selectSubtree( pipe->GetEndKnotPtr(), op ); 
			} 
		);
	}
}

void NNetModel::deleteShape( Shape * const pShape )
{
	ShapeId id { pShape->GetId() };
	assert( m_Shapes[ id.GetValue() ] == pShape );
	m_Shapes[ id.GetValue() ] = nullptr;  // first remove anchor
	delete pShape;                        // then delete Shape
}

void NNetModel::ConnectIncoming
( 
	Pipe     * const pPipe, 
	BaseKnot * const pEndPoint
)
{
	if ( pPipe && pEndPoint )
	{
		pEndPoint->LockShapeExclusive();
		pEndPoint->AddIncoming( pPipe );
		pPipe->SetEndKnot( pEndPoint );
		pEndPoint->UnlockShapeExclusive();
	}
}

void NNetModel::ConnectOutgoing
( 
	Pipe     * const pPipe, 
	BaseKnot * const pStartPoint
)
{
	if ( pPipe && pStartPoint )
	{
		pStartPoint->LockShapeExclusive();
		pStartPoint->AddOutgoing( pPipe );
		pPipe->SetStartKnot( pStartPoint );
		pStartPoint->UnlockShapeExclusive();
	}
}

void NNetModel::disconnectBaseKnot( BaseKnot * const pBaseKnot ) // disconnects only, shape remains
{
	if ( pBaseKnot )
	{
		MicroMeterPoint umPos { pBaseKnot->GetPosition() };
		pBaseKnot->Apply2AllInPipes_Lock
		( 
			[&]( Pipe * const pPipe ) // every incoming Pipe needs a new end knot
			{ 
				Knot * pKnotNew { NewShape<Knot>( umPos ) };

				pKnotNew->LockShapeExclusive();
				pKnotNew->AddIncoming( pPipe );
				pPipe->SetEndKnot( pKnotNew );
				pKnotNew->UnlockShapeExclusive();

				pPipe->DislocateEndPoint( );
				pPipe->Recalc();
			} 
		);
		pBaseKnot->ClearIncoming();
		pBaseKnot->Apply2AllOutPipes_Lock
		( 
			[&]( Pipe * const pPipe ) // every outgoing Pipe needs a new start knot
			{ 
				Knot * pKnotNew { NewShape<Knot>( umPos ) };

				pKnotNew->LockShapeExclusive();
				pKnotNew->AddOutgoing( pPipe );
				pPipe->SetStartKnot( pKnotNew );
				pKnotNew->UnlockShapeExclusive();

				pPipe->DislocateStartPoint( );
				pPipe->Recalc();
			} 
		);
		pBaseKnot->ClearOutgoing();
		assert( pBaseKnot->IsOrphan( ) );
	}
}

void NNetModel::deletePipeEndPoints( Pipe * const pPipe )
{
	BaseKnot * pStartKnot { pPipe->GetStartKnotPtr() };
	pStartKnot->RemoveOutgoing( pPipe );
	if ( pStartKnot->IsOrphanedKnot( ) && ( pStartKnot->IsKnot() ) )
		deleteShape( pStartKnot );

	BaseKnot * pEndKnot { pPipe->GetEndKnotPtr() };
	pEndKnot->RemoveIncoming( pPipe );
	if ( pEndKnot->IsOrphanedKnot( ) && ( pEndKnot->IsKnot() ) )
		deleteShape( pEndKnot );
}

void NNetModel::insertBaseKnot( Pipe * const pPipe, BaseKnot * const pBaseKnot)
{
	if ( pPipe && pBaseKnot )
	{
		pPipe->LockShapeExclusive();
		BaseKnot * const pStartKnot { pPipe->GetStartKnotPtr( ) };
		NewPipe( pStartKnot, pBaseKnot );
		pStartKnot->RemoveOutgoing( pPipe );
		ConnectOutgoing( pPipe, pBaseKnot );
		pPipe->Recalc();
		pPipe->UnlockShapeExclusive();
	}
}

MicroMeterPoint NNetModel::orthoVector( ShapeId const idPipe ) const
{
	return OrthoVector( GetShapeConstPtr<Pipe const *>( idPipe )->GetVector(), NEURON_RADIUS * 2.f );
}

ShapeId const NNetModel::FindShapeAt
( 
	MicroMeterPoint const   pnt, 
	ShapeCrit       const & crit 
) const
{
	for ( size_t i = m_Shapes.size(); i --> 0; )	
	{
		Shape * pShape = m_Shapes[i];
		if ( pShape && crit( * pShape ) && pShape->IsPointInShape( pnt ) ) 
			return pShape->GetId();
	};
	return NO_SHAPE;
}

/////////////////// nonmember functions ///////////////////////////////////////////////

MicroMeterRect GetEnclosingRect( ShapeList const & shapeVector )
{
	MicroMeterRect rect { MicroMeterRect::ZERO_VAL() };
	for ( const auto & pShape : shapeVector )
	{
		if ( pShape )
		{
			if ( pShape->IsBaseKnot() )
			{
				rect.Expand( Cast2BaseKnot( pShape )->GetPosition() );
			}
			else if ( pShape->IsPipe() )
			{
				rect.Expand( Cast2Pipe( pShape )->GetStartPoint() );
				rect.Expand( Cast2Pipe( pShape )->GetEndPoint() );
			}
			else
			{
				assert( false );
			}
		}
	}
	return rect;
}
