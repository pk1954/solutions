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
#include "NNetModel.h"

using namespace std::chrono;
using std::unordered_map;

NNetModel::NNetModel( NNetModel const & modelSrc )
{
	* this = modelSrc;
	for ( auto pShapeSrc : modelSrc.m_Shapes )
	{
		if ( pShapeSrc )
		{
			m_Shapes.at( pShapeSrc->GetId().GetValue() ) = ShallowCopy( * pShapeSrc );
		}
	}
	for ( auto pShapeSrc : modelSrc.m_Shapes )
	{
		if ( pShapeSrc )
		{
			ConnectToNewShapes( * pShapeSrc, m_Shapes );
		}
	}
	assert( IsEqual( modelSrc ) );
}

NNetModel::~NNetModel( )
{
	for ( auto pShape : m_Shapes )
		delete pShape;
	m_Shapes.clear();
}

void NNetModel::Initialize
(
	Param      * const pParam, 
	Observable * const pStaticModelObservable,
	Observable * const pDynamicModelObservable,
	Observable * const pModelTimeObservable
)
{					
	m_pParam                  = pParam;
	m_pStaticModelObservable  = pStaticModelObservable;
    m_pDynamicModelObservable = pDynamicModelObservable;
	m_pModelTimeObservable    = pModelTimeObservable;
	Shape::SetParam( pParam );
}                     

void NNetModel::CreateInitialShapes( )
{
	InputNeuron * const pInputNeuron { NewShape<InputNeuron >( MicroMeterPoint( 400.0_MicroMeter, 200.0_MicroMeter ) ) };
	Neuron      * const pNeuron      { NewShape<Neuron>      ( MicroMeterPoint( 400.0_MicroMeter, 800.0_MicroMeter ) ) };
	Pipe        * const pNewPipe     { new Pipe( pInputNeuron, pNeuron ) };
	pInputNeuron->m_connections.AddOutgoing( pNewPipe );
	pNeuron  ->m_connections.AddIncoming( pNewPipe );
	Add2ShapeList( pNewPipe );
	StaticModelChanged( );
}

bool NNetModel::IsEqual( NNetModel const & other ) const
{
	size_t iMax { max( m_Shapes.size(), other.m_Shapes.size() ) };
	for ( int i = 0; i < iMax; ++i )
	{
		Shape const * pShape      { (i >=       m_Shapes.size()) ? nullptr :       m_Shapes[i] };
		Shape const * pShapeOther { (i >= other.m_Shapes.size()) ? nullptr : other.m_Shapes[i] };
		if ( (pShape == nullptr) != (pShapeOther == nullptr) )
		{
			return false;
		}
		else if ( (pShape != nullptr) && (pShapeOther != nullptr) )
		{
			if ( ! isEqual( * pShape, * pShapeOther ) )
				return false;
		}
	}
	return true;
}

void NNetModel::RecalcAllShapes( ) 
{ 
	Apply2All<Shape>( [&]( Shape & shape ) { shape.Recalc( ); } );
	dynamicModelChanged( );
} 

void NNetModel::DoDeleteBaseKnot( BaseKnot * const pBaseKnot )
{
	MicroMeterPoint umPos { pBaseKnot->GetPosition() };
	pBaseKnot->m_connections.Apply2AllInPipes
	( 
		[&]( Pipe & pipe ) // every incoming Pipe needs a new end knot
		{ 
			Knot * pKnotNew { NewShape<Knot>( umPos ) };
			pKnotNew->m_connections.AddIncoming( & pipe );
			pipe.SetEndKnot( pKnotNew );
			pipe.DislocateEndPoint( );
			pipe.Recalc();
		} 
	);
	pBaseKnot->m_connections.ClearIncoming();
	pBaseKnot->m_connections.Apply2AllOutPipes
	( 
		[&]( Pipe & pipe ) // every outgoing Pipe needs a new start knot
		{ 
			Knot * pKnotNew { NewShape<Knot>( umPos ) };
			pKnotNew->m_connections.AddOutgoing( & pipe );
			pipe.SetStartKnot( pKnotNew );
			pipe.DislocateStartPoint( );
			pipe.Recalc();
		} 
	);
	pBaseKnot->m_connections.ClearOutgoing();
	assert( pBaseKnot->m_connections.IsOrphan( ) );

	RemoveFromShapeList( pBaseKnot );
}

void NNetModel::UndoDeleteBaseKnot( BaseKnot * const pBaseKnot )
{
	MicroMeterPoint umPos { pBaseKnot->GetPosition() };
	pBaseKnot->m_connections.Apply2AllInPipes
	( 
		[&]( Pipe & pipe ) // reconnect to original end knot
		{ 
			BaseKnot * pEndPoint { pipe.GetEndKnotPtr() };
			pipe.SetEndKnot( pBaseKnot );
			pipe.Recalc();
			delete pEndPoint;
		} 
	);
	pBaseKnot->m_connections.Apply2AllOutPipes
	( 
		[&]( Pipe & pipe ) // reconnect to original start knot
		{ 
			BaseKnot * pStartPoint { pipe.GetStartKnotPtr() };
			pipe.SetStartKnot( pBaseKnot );
			pipe.Recalc();
			delete pStartPoint;
		} 
	);
	Restore2ShapeList( pBaseKnot );
}

void NNetModel::DoDeletePipe
( 
	Pipe * const pPipe
)
{
	BaseKnot * const pStartKnot = pPipe->GetStartKnotPtr();
	BaseKnot * const pEndKnot   = pPipe->GetEndKnotPtr();
	pStartKnot->m_connections.RemoveOutgoing( pPipe );
	if ( pStartKnot->IsOrphanedKnot( ) )
		RemoveFromShapeList( pStartKnot );

	pEndKnot->m_connections.RemoveIncoming( pPipe );
	if ( pEndKnot->IsOrphanedKnot( ) )
		RemoveFromShapeList( pEndKnot );

	RemoveFromShapeList( pPipe );
}

void NNetModel::UndoDeletePipe
( 
	Pipe * const pPipe
)
{
	BaseKnot * const pStartKnot = pPipe->GetStartKnotPtr();
	BaseKnot * const pEndKnot   = pPipe->GetEndKnotPtr();
	pStartKnot->m_connections.AddOutgoing( pPipe );
	pEndKnot  ->m_connections.AddIncoming( pPipe );
	Restore2ShapeList( pStartKnot );
	Restore2ShapeList( pEndKnot );
	Restore2ShapeList( pPipe );
}

void NNetModel::DeleteShape( Shape * const pShape )
{
	if ( pShape )
	{
		if ( pShape->IsPipe() )
		{
			Pipe     * const pPipe      { Cast2Pipe( pShape ) };
			BaseKnot * const pStartKnot { pPipe->GetStartKnotPtr() };
			pStartKnot->m_connections.RemoveOutgoing( pPipe );
			if ( pStartKnot->IsOrphanedKnot( ) )
			{
				RemoveFromShapeList( pStartKnot );
				delete pStartKnot;                       
			}

			BaseKnot * pEndKnot { pPipe->GetEndKnotPtr() };
			pEndKnot->m_connections.RemoveIncoming( pPipe );
			if ( pEndKnot->IsOrphanedKnot( ) )
			{
				RemoveFromShapeList( pEndKnot );
				delete( pEndKnot );
			}
		}
		else 
			DisconnectBaseKnot( Cast2BaseKnot( pShape ) );
		RemoveFromShapeList( pShape );
		delete pShape;
	}
}

void NNetModel::ToggleStopOnTrigger( ShapeId const id )
{
	if ( Neuron * pNeuron { GetShapePtr<Neuron *>( id ) } )
	{
		pNeuron->StopOnTrigger( tBoolOp::opToggle );
		dynamicModelChanged( );
	}
}

fHertz const NNetModel::GetPulseRate( ShapeId const id ) const
{
	InputNeuron const * const pInputNeuron { GetShapeConstPtr<InputNeuron const *>( id ) };
	return ( pInputNeuron )
		   ? pInputNeuron->GetPulseFrequency( )
	       : fHertz::NULL_VAL();
}

float NNetModel::SetParameter
( 
	tParameter const param, 
	float      const fNewValue 
)
{
	float fOldValue { m_pParam->GetParameterValue( param ) };
	m_pParam->SetParameterValue( param, fNewValue );
	RecalcAllShapes( );
	return fOldValue;
}

MicroMeterPoint const NNetModel::GetShapePos( ShapeId const id ) const
{
	if ( BaseKnot const * pBaseKnot = GetShapeConstPtr<BaseKnot const *>( id ) )
		return pBaseKnot->GetPosition();
	return NP_NULL;
}

bool NNetModel::Compute( )
{
	bool bStop {false };
	Apply2All<Shape>( [&]( Shape & shape ) { shape.Prepare( ); } );
	Apply2All<Shape>( [&]( Shape & shape ) { if ( shape.CompStep( ) ) bStop = true; } );
	dynamicModelChanged();
	incTimeStamp( );
	return bStop;
}

void NNetModel::ResetModel( )
{
	for (auto pShape : m_Shapes)
		delete pShape;
	m_Shapes.clear();
	StaticModelChanged( );
}

void NNetModel::DeleteSelection( )
{
	for ( int i = 0; i < m_Shapes.size(); ++i )  // Caution!
	{	                                         // Range based loop does not work here
		Shape * p = m_Shapes.at(i);              // DeleteShape changes the range 
		if ( p &&  p->IsSelected() )             // by creating new shapes!!
			DeleteShape( p ); 
	}
}

bool NNetModel::isEqual( Shape const & shapeA, Shape const & shapeB ) const
{
	if ( shapeA.GetShapeType().GetValue() != shapeB.GetShapeType().GetValue() )
		return false;

	switch ( shapeA.GetShapeType().GetValue() )
	{
	case ShapeType::Value::inputNeuron:
		if (! IS_EQUAL<InputNeuron>( shapeA, shapeB ) ) 
			return false;
		break;

	case ShapeType::Value::knot:
		if (! IS_EQUAL<Knot>( shapeA, shapeB ) ) 
			return false;
		break;

	case ShapeType::Value::neuron:
		if (! IS_EQUAL<Neuron>( shapeA, shapeB ) ) 
			return false;
		break;

	case ShapeType::Value::pipe:
		if (! IS_EQUAL<Pipe>( shapeA, shapeB ) ) 
			return false;
		break;

	default:
		assert( false );
		return false;
	}
	return true;
}

Shape * NNetModel::ShallowCopy( Shape const & shape ) const
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

void NNetModel::ConnectToNewShapes( Shape const & shapeSrc, ShapeList const & newShapes ) const 
{ 
	Shape & shapeDst { * newShapes.at(shapeSrc.GetId().GetValue()) };
	if ( shapeSrc.IsPipe( ) )
	{
		Pipe const & pipeSrc { static_cast<Pipe const &>( shapeSrc ) };
		Pipe       & pipeDst { static_cast<Pipe       &>( shapeDst ) };
		pipeDst.SetStartKnot( static_cast<BaseKnot *>( newShapes.at( pipeSrc.GetStartKnotId().GetValue() )) );
		pipeDst.SetEndKnot  ( static_cast<BaseKnot *>( newShapes.at( pipeSrc.GetEndKnotId  ().GetValue() )) );
		pipeDst.Recalc();
	}
	else
	{
		Connections const & srcConn { static_cast<BaseKnot const &>( shapeSrc ).m_connections };
		Connections       & dstConn { static_cast<BaseKnot       &>( shapeDst ).m_connections };
		dstConn.ClearOutgoing();
		dstConn.ClearIncoming();
		auto dstFromSrc = [&](Pipe const & pipeSrc){ return static_cast<Pipe *>(newShapes.at(pipeSrc.GetId().GetValue())); };
		srcConn.Apply2AllOutPipes( [&]( Pipe const & pipeSrc ) { dstConn.AddOutgoing( dstFromSrc( pipeSrc ) ); } );
		srcConn.Apply2AllInPipes ( [&]( Pipe const & pipeSrc ) { dstConn.AddIncoming( dstFromSrc( pipeSrc ) ); } );
	}
}

void NNetModel::SelectSubtree( BaseKnot * const pBaseKnot, tBoolOp const op )
{
	if ( pBaseKnot )
	{
		pBaseKnot->Select( op );
		pBaseKnot->m_connections.Apply2AllOutPipes
		( 
			[&]( Pipe & pipe ) 
			{ 
				pipe.Select( op ); 
				if ( pipe.GetEndKnotPtr()->IsKnot() )
					SelectSubtree( pipe.GetEndKnotPtr(), op ); 
			} 
		);
	}
}

void NNetModel::DisconnectBaseKnot( BaseKnot * const pBaseKnot ) // disconnects only, shape remains
{
	if ( pBaseKnot )
	{
		MicroMeterPoint umPos { pBaseKnot->GetPosition() };
		pBaseKnot->m_connections.Apply2AllInPipes
		( 
			[&]( Pipe & pipe ) // every incoming Pipe needs a new end knot
			{ 
				Knot * pKnotNew { NewShape<Knot>( umPos ) };
				pKnotNew->m_connections.AddIncoming( & pipe );
				pipe.SetEndKnot( pKnotNew );
				pipe.DislocateEndPoint( );
				pipe.Recalc();
			} 
		);
		pBaseKnot->m_connections.ClearIncoming();
		pBaseKnot->m_connections.Apply2AllOutPipes
		( 
			[&]( Pipe & pipe ) // every outgoing Pipe needs a new start knot
			{ 
				Knot * pKnotNew { NewShape<Knot>( umPos ) };
				pKnotNew->m_connections.AddOutgoing( & pipe );
				pipe.SetStartKnot( pKnotNew );
				pipe.DislocateStartPoint( );
				pipe.Recalc();
			} 
		);
		pBaseKnot->m_connections.ClearOutgoing();
		assert( pBaseKnot->m_connections.IsOrphan( ) );
	}
}

ShapeId const NNetModel::FindShapeAt
( 
	MicroMeterPoint const & umPoint, 
	ShapeCrit       const & crit 
) const
{	
	ShapeId idRes { NO_SHAPE };

	if ( idRes == NO_SHAPE )   // first test all neurons and input neurons
		idRes = findShapeAt( umPoint, [&]( Shape const & s ) { return s.IsAnyNeuron( ) && crit( s ); } );

	if ( idRes == NO_SHAPE )   // if nothing found, test knot shapes
		idRes = findShapeAt( umPoint, [&]( Shape const & s ) { return s.IsKnot     ( ) && crit( s ); } ); 	

	if ( idRes == NO_SHAPE )   // if nothing found, try pipes
		idRes = findShapeAt( umPoint, [&]( Shape const & s ) { return s.IsPipe     ( ) && crit( s ); } );

	return idRes;
}

/////////////////// local functions ///////////////////////////////////////////////

ShapeId const NNetModel::findShapeAt
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

MicroMeterRect ComputeEnclosingRect( ShapeList const & shapeVector )
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

void ConnectIncoming
( 
	Pipe     * const pPipe, 
	BaseKnot * const pEndPoint
)
{
	pEndPoint->m_connections.AddIncoming( pPipe );
	pPipe->SetEndKnot( pEndPoint );
	pPipe->Recalc( );
}

void ReplaceStartKnot
( 
	Pipe     * const pPipe, 
	BaseKnot * const pStartPoint
)
{
	BaseKnot * const pOldStartPoint { pPipe->GetStartKnotPtr() };
	pOldStartPoint->m_connections.RemoveIncoming( pPipe ); 
	pPipe->SetStartKnot( pStartPoint );
	pPipe->Recalc( );
}

void ReplaceEndKnot  
( 
	Pipe     * const pPipe, 
	BaseKnot * const pEndPoint
)
{
	BaseKnot * const pOldEndPoint { pPipe->GetEndKnotPtr() };
	pOldEndPoint->m_connections.RemoveIncoming( pPipe ); 
	pPipe->SetEndKnot( pEndPoint );
	pPipe->Recalc( );
}
