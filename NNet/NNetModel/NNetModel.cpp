// NNetModel.cpp 
//
// NNetModel

#include "stdafx.h"
#include "scanner.h"
#include <vector>
#include <unordered_map>
#include "MoreTypes.h"
#include "tHighlightType.h"
#include "ShapeType.h"
#include "Knot.h"
#include "Neuron.h"
#include "InputNeuron.h"
#include "NNetModel.h"

using namespace std::chrono;
using std::unordered_map;
using std::wcout;
using std::endl;

bool NNetModel::operator==( NNetModel const & rhs ) const
{
	return
	(m_Shapes                    == rhs.m_Shapes                    ) &&
	(m_timeStamp                 == rhs.m_timeStamp                 ) &&
	(m_pStaticModelObservable    == rhs.m_pStaticModelObservable    ) &&
	(m_pDynamicModelObservable   == rhs.m_pDynamicModelObservable   ) &&
    (m_pUnsavedChangesObservable == rhs.m_pUnsavedChangesObservable ) &&
	(m_enclosingRect             == rhs.m_enclosingRect             ) &&
	(m_wstrModelFilePath         == rhs.m_wstrModelFilePath         ) &&
	(m_description               == rhs.m_description               ) &&
	(m_monitorData               == rhs.m_monitorData               ) &&
	(m_param                     == rhs.m_param                     );
}

void NNetModel::Initialize
(
	Observable * const pStaticModelObservable,
	Observable * const pDynamicModelObservable,
	Observable * const pUnsavedChangesObservable
)
{				
	m_pStaticModelObservable    = pStaticModelObservable;
    m_pDynamicModelObservable   = pDynamicModelObservable;
	m_pUnsavedChangesObservable = pUnsavedChangesObservable;
}                     

void NNetModel::CheckModel( ) const
{
#ifdef _DEBUG
	m_Shapes.CheckShapeList( );
#endif
}

Shape const * NNetModel::GetConstShape( ShapeId const id ) const 
{	
	if ( IsUndefined( id ) || ! m_Shapes.IsValidShapeId( id ) )
	{
		DumpModel();
		m_Shapes.CallErrorHandler( id );  
		return nullptr;
	}
	return m_Shapes.GetAt( id );
}

void NNetModel::SetSimulationTime( fMicroSecs const newVal )	
{ 
	m_timeStamp = newVal; 
	m_pDynamicModelObservable->NotifyAll( false );
}

void NNetModel::StaticModelChanged( )
{ 
	m_pStaticModelObservable->NotifyAll( false );
	m_enclosingRect = m_Shapes.EnclosingRect( );
}

void NNetModel::RecalcAllShapes( ) 
{ 
	m_Shapes.Apply2All( [&]( Shape & shape ) { shape.Recalc( ); } );
} 

void NNetModel::ToggleStopOnTrigger( Neuron * pNeuron )
{
	pNeuron->StopOnTrigger( tBoolOp::opToggle );
	m_pStaticModelObservable->NotifyAll( false );
}

fHertz const NNetModel::GetPulseRate( ShapeId const id ) const
{
	InputNeuron const * const pInputNeuron { GetShapeConstPtr<InputNeuron const *>( id ) };
	return ( pInputNeuron )
		   ? pInputNeuron->GetPulseFrequency( )
	       : fHertz::NULL_VAL();
}

float NNetModel::SetParam
( 
	ParameterType::Value const param, 
	float      const fNewValue 
)
{
	float fOldValue { m_param.GetParameterValue( param ) };
	m_param.SetParameterValue( param, fNewValue );
	RecalcAllShapes( );
	StaticModelChanged( );
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
	m_timeStamp += m_param.GetTimeResolution( );
	m_Shapes.Apply2All( [&](Shape &s) { s.Prepare( ); } );
	m_Shapes.Apply2All( [&](Shape &s) { if ( s.CompStep( ) ) bStop = true; } );
	m_pDynamicModelObservable->NotifyAll( false );
	return bStop;
}

void NNetModel::ResetModel( )
{
	m_wstrModelFilePath = L""; 
	m_Shapes.Clear();
	m_monitorData.Reset();
	Knot       ::ResetCounter();
	Neuron     ::ResetCounter();
	InputNeuron::ResetCounter();
	Pipe       ::ResetCounter();
	SetSimulationTime();
	StaticModelChanged( );
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

ShapeId const NNetModel::FindShapeAt
( 
	MicroMeterPoint const & umPoint, 
	ShapeCrit       const & crit 
) const
{	
	ShapeId idRes { NO_SHAPE };

	if ( idRes == NO_SHAPE )   // first test all neurons and input neurons
		idRes = m_Shapes.FindShapeAt( umPoint, [&]( Shape const & s ) { return s.IsAnyNeuron( ) && crit( s ); } );

	if ( idRes == NO_SHAPE )   // if nothing found, test knot shapes
		idRes = m_Shapes.FindShapeAt( umPoint, [&]( Shape const & s ) { return s.IsKnot     ( ) && crit( s ); } ); 	

	if ( idRes == NO_SHAPE )   // if nothing found, try pipes
		idRes = m_Shapes.FindShapeAt( umPoint, [&]( Shape const & s ) { return s.IsPipe     ( ) && crit( s ); } );

	return idRes;
}

void NNetModel::DumpModel( ) const
{
	wcout << Scanner::COMMENT_SYMBOL << L"------------ Dump start ------------" << endl;
	m_Shapes.Dump();
	wcout << Scanner::COMMENT_SYMBOL << L"------------ Dump end ------------" << endl;
}
